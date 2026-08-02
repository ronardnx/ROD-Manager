#!/system/bin/sh
OMK_DIR="/data/misc/keystore/omk"
INJECTOR_TOML="$OMK_DIR/injector.toml"
KEYBOX_XML="$OMK_DIR/keybox.xml"
OMK_RESTART_DIR="/data/adb/omk"
die() { echo "ERROR: $*" >&2; exit 1; }

STD_ALPHABET="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"
SHUFFLED_ALPHABET="1dgWnocayqxU3r6vA5lCIPYfHmkV08b4tz+KMsp2NQ9LRXihODwSj7BEFJ/ZuGTe"

decode_substitution() {
  tr "$SHUFFLED_ALPHABET" "$STD_ALPHABET" < "$1" > "$2"
}

_parse_serial() {
  _h="$1"
  case "${_h:0:1}" in "") return 1 ;; esac 2>/dev/null || { return 1; }
  case "$_h" in 30*) _h="${_h#30}" ;; *) return 1 ;; esac
  _l_hex="${_h:0:2}" _l_dec=$((16#$_l_hex))
  [ $_l_dec -ge 128 ] && _h="${_h:2 + ($_l_dec - 128) * 2}" || _h="${_h:2}"
  case "$_h" in 30*) _h="${_h#30}" ;; *) return 1 ;; esac
  _l_hex="${_h:0:2}" _l_dec=$((16#$_l_hex))
  [ $_l_dec -ge 128 ] && _h="${_h:2 + ($_l_dec - 128) * 2}" || _h="${_h:2}"
  case "$_h" in
    a0*)
      _ctx_len_hex="${_h:2:2}"
      _ctx_len=$((16#$_ctx_len_hex))
      _h="${_h:4 + _ctx_len * 2}"
      ;;
  esac
  case "$_h" in 02*) _h="${_h#02}" ;; *) return 1 ;; esac
  _l_hex="${_h:0:2}" _l_dec=$((16#$_l_hex))
  if [ $_l_dec -ge 128 ]; then
    _n=$((_l_dec - 128))
    _sl=$((16#${_h:2:_n * 2}))
    _serial_hex="${_h:2 + _n * 2:$_sl * 2}"
  else
    _serial_hex="${_h:2:$_l_dec * 2}"
  fi
  _serial=$(echo "$_serial_hex" | sed 's/^0*//')
  [ -z "$_serial" ] && _serial="0"
  return 0
}

decode_keybox_serial() {
  _b64=$(sed -n '/-----BEGIN CERTIFICATE-----/,/-----END CERTIFICATE-----/p; /-----END CERTIFICATE-----/q' "$1" | grep -v 'CERTIFICATE' | sed 's/^[[:space:]]*//' | tr -d '\n')
  [ -z "$_b64" ] && return 1
  _hex=$(echo "$_b64" | base64 -d 2>/dev/null | od -v -tx1 | awk 'BEGIN{ORS=""} {for(i=2;i<=NF;i++) printf "%s", $i}')
  [ -z "$_hex" ] && return 1
  _parse_serial "$_hex" || return 1
  echo "$_serial"
}


_download() {
    local url="$1"
    if command -v curl >/dev/null 2>&1; then
        curl -sSL "$url"
    elif command -v wget >/dev/null 2>&1; then
        wget -qO- "$url"
    elif command -v busybox >/dev/null 2>&1; then
        busybox wget -qO- "$url"
    else
        die "No curl or wget found!"
    fi
}

cmd_fetch_specter_keybox() {
    local source="$1"
    local version="$2"
    [ -z "$source" ] || [ -z "$version" ] && die "Usage: fetch_specter_keybox <source> <version>"
    
    local url="https://rawbin.dpejoh.com/key/${source}/${version}"
    local tmp_enc="/data/local/tmp/specter_kb_$$.enc"
    local tmp_dec="/data/local/tmp/specter_kb_$$.dec"
    
    _download "$url" > "$tmp_enc" || { rm -f "$tmp_enc"; die "Failed to download keybox"; }
    decode_substitution "$tmp_enc" "$tmp_dec" || { rm -f "$tmp_enc" "$tmp_dec"; die "Substitution decode failed"; }
    
    mkdir -p "$OMK_DIR"
    base64 -d < "$tmp_dec" > "$KEYBOX_XML" 2>/dev/null || { rm -f "$tmp_enc" "$tmp_dec"; die "Base64 decode failed"; }
    
    chown system:system "$KEYBOX_XML" 2>/dev/null
    chmod 600 "$KEYBOX_XML"
    
    rm -f "$tmp_enc" "$tmp_dec"
    _reload_omk
    echo "OK: Specter keybox installed ($source v$version)"
}

cmd_check_revocation() {
    [ ! -f "$KEYBOX_XML" ] && die "No keybox.xml found"
    local serial
    serial=$(decode_keybox_serial "$KEYBOX_XML")
    [ -z "$serial" ] && die "Failed to parse certificate serial from keybox"
    
    local url="https://android.googleapis.com/attestation/status?encrypted=0"
    local resp
    resp=$(_download "$url" 2>/dev/null)
    [ -z "$resp" ] && die "Failed to fetch Google attestation status"
    
    if echo "$resp" | grep -q "\"$serial\""; then
        echo "REVOKED"
    else
        if command -v bc >/dev/null 2>&1; then
            local dec=$(echo "ibase=16; $(echo "$serial" | tr 'a-f' 'A-F')" | bc 2>/dev/null)
            if [ -n "$dec" ] && echo "$resp" | grep -q "\"$dec\""; then
                echo "REVOKED"
                return
            fi
        fi
        echo "VALID"
    fi
}

cmd_status() {
    local installed="false"
    local has_keybox="false"
    [ -f "$INJECTOR_TOML" ] && installed="true"
    [ -f "$KEYBOX_XML" ] && has_keybox="true"
    echo "{"
    echo "  \"installed\": $installed,"
    echo "  \"has_keybox\": $has_keybox,"
    echo "  \"injector_path\": \"$INJECTOR_TOML\""
    echo "}"
}
_write_scoop() {
    local file="$1"
    shift
    [ -f "$file" ] || die "injector.toml not found at $file — is OhMyKeymint installed and has it run once?"
    local block_file="${file}.block.$$"
    local tmp_file="${file}.new.$$"
    printf 'scoop = [\n' > "$block_file"
    for pkg in "$@"; do
        [ -z "$pkg" ] && continue
        printf '  "%s",\n' "$pkg" >> "$block_file"
    done
    printf ']\n' >> "$block_file"
    if grep -Eq '^[ ]*scoop[ ]*=' "$file"; then
        awk -v blockfile="$block_file" '
          function emit(  line) { while ((getline line < blockfile) > 0) print line; close(blockfile) }
          {
            if (capture) { if ($0 ~ /\]/) capture = 0; next }
            if ($0 ~ /^[ ]*scoop[ ]*=/) {
              emit()
              if ($0 !~ /\]/) capture = 1
              next
            }
            print
          }
        ' "$file" > "$tmp_file"
    elif grep -Eq '^[ ]*\[' "$file"; then
        awk -v blockfile="$block_file" '
          function emit(  line) { while ((getline line < blockfile) > 0) print line; close(blockfile) }
          BEGIN { injected = 0 }
          {
            if (!injected && $0 ~ /^[ ]*\[/) { emit(); injected = 1 }
            print
          }
        ' "$file" > "$tmp_file"
    else
        cat "$block_file" > "$tmp_file"
    fi
    cat "$tmp_file" > "$file"
    rm -f "$tmp_file" "$block_file"
}
_reload_omk() {
    mkdir -p "$OMK_RESTART_DIR" 2>/dev/null || true
    touch "$OMK_RESTART_DIR/restart.keymint" 2>/dev/null || true
    touch "$OMK_RESTART_DIR/restart.injector" 2>/dev/null || true
    touch "$OMK_RESTART_DIR/restart.all" 2>/dev/null || true
}
cmd_sync_denylist() {
    [ -f "$INJECTOR_TOML" ] || die "injector.toml not found at $INJECTOR_TOML. Boot the device with OMK installed first."
    rm -f "${INJECTOR_TOML}.bak" "${INJECTOR_TOML}.rod.bak" 2>/dev/null || true
    local pkgs="com.android.vending
com.google.android.gsf
com.google.android.gms
com.google.android.gms.unstable"
    local user_apps
    user_apps=$(pm list packages -3 2>/dev/null | sed 's/^package://' | tr -d '\r' | grep -v '^$')
    if [ -z "$user_apps" ]; then
        user_apps=$(ls /data/app 2>/dev/null | sed 's/-[^-]*$//' | grep '\.' | sort -u)
    fi
    for pkg in $user_apps; do
        [ -z "$pkg" ] && continue
        case "$pkg" in
            com.android.vending|com.google.android.gsf|com.google.android.gms|com.google.android.gms.unstable) continue ;;
        esac
        pkgs="$pkgs
$pkg"
    done
    _write_scoop "$INJECTOR_TOML" $pkgs
    _reload_omk
    local count
    count=$(echo "$pkgs" | grep -c .)
    echo "OK: scoop updated with $count packages and OMK reload signalled"
}
cmd_import_keybox() {
    local b64_data="$1"
    [ -z "$b64_data" ] && die "No Base64 data provided"
    mkdir -p "$OMK_DIR"
    echo "$b64_data" | base64 -d > "$KEYBOX_XML"
    chown system:system "$KEYBOX_XML" 2>/dev/null
    chmod 600 "$KEYBOX_XML"
    _reload_omk
    echo "OK: keybox.xml imported and OMK reload signalled"
}
cmd_restart_all() {
    _reload_omk
    echo "OK: OMK reload signals written to $OMK_RESTART_DIR"
}
CMD="${1:-status}"
shift 2>/dev/null
case "$CMD" in
    status)               cmd_status "$@" ;;
    sync_denylist)        cmd_sync_denylist "$@" ;;
    import_keybox)        cmd_import_keybox "$@" ;;
    restart_all)          cmd_restart_all "$@" ;;
    fetch_specter_keybox) cmd_fetch_specter_keybox "$@" ;;
    check_revocation)     cmd_check_revocation "$@" ;;
    fetch_catalog)        _download "https://rawbin.dpejoh.com/catalog" ;;
    *) die "Unknown command: $CMD" ;;
esac
