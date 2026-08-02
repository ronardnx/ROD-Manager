#!/system/bin/sh
PATH=/data/adb/ksu/bin:$PATH
MODDIR="/data/adb/modules/rod"
SUSFS_BIN="$MODDIR/webroot/susfs"
KSU_BIN="/data/adb/ksud"
PERSISTENT_DIR="/data/adb/rod"
CONFIG_FILE="$PERSISTENT_DIR/susfs_config.sh"
die() { echo "ERROR: $*" >&2; exit 1; }
init_config() {
    mkdir -p "$PERSISTENT_DIR"
    if [ ! -f "$PERSISTENT_DIR/sus_kstat_statically.json" ]; then
        cat > "$PERSISTENT_DIR/sus_kstat_statically.json" <<'EOF'
{
  "path": "/data/local/tmp",
  "ino": "4521",
  "dev": "2063",
  "nlink": "2",
  "size": "4096",
  "atime": "default",
  "atime_nsec": "default",
  "mtime": "default",
  "mtime_nsec": "default",
  "ctime": "default",
  "ctime_nsec": "default",
  "blocks": "8",
  "blksize": "4096"
}
EOF
    fi
    [ -f "$CONFIG_FILE" ] && return 0
    cat > "$CONFIG_FILE" <<'EOF'
hide_sus_mnts_for_non_su_procs=1
spoof_cmdline=1
spoof_uname=0
kernel_version=default
kernel_build=default
enable_avc_log_spoofing=1
su_compat=1
kernel_umount=1
selinux_hide=1
developer_options=0
usb_debugging=0
wireless_debugging=0
selinux_enforcing=1
EOF
    touch "$PERSISTENT_DIR/sus_path.txt"
    touch "$PERSISTENT_DIR/sus_path_loop.txt"
    touch "$PERSISTENT_DIR/sus_maps.txt"
}
get_cfg() {
    local val
    val=$(grep "^$1=" "$CONFIG_FILE" 2>/dev/null | cut -d= -f2- | tr -d '\r')
    echo "${val:-0}"
}
set_cfg() {
    if grep -q "^$1=" "$CONFIG_FILE" 2>/dev/null; then
        sed -i "s|^$1=.*|$1=$2|" "$CONFIG_FILE"
    else
        echo "$1=$2" >> "$CONFIG_FILE"
    fi
}
susfs_run() { "$SUSFS_BIN" "$@" 2>/dev/null; }
check_susfs() {
    [ -x "$SUSFS_BIN" ] || return 1
    local f; f=$(susfs_run show enabled_features)
    [ -n "$f" ] && return 0
    dmesg 2>/dev/null | grep -q "susfs:" && return 0
    return 1
}
spoof_uname_fn() {
    local kver; kver=$(get_cfg kernel_version)
    local kbuild; kbuild=$(get_cfg kernel_build)
    
    if [ "$kver" = "default" ] || [ -z "$kver" ]; then
        local current=$(uname -r)
        local major=$(echo "$current" | cut -d. -f1)
        local minor=$(echo "$current" | cut -d. -f2)
        if [ "$major" -gt 5 ] || { [ "$major" -eq 5 ] && [ "$minor" -ge 10 ]; }; then
            # Extract kernel version, android release, and optional KMI number (e.g. 6.1.141-android14-11 or 6.1.141-android14)
            local base_gki=$(echo "$current" | grep -oE -- '^[0-9]+\.[0-9]+\.[0-9]+-android[0-9]+(-[0-9]+)?' | head -n1)
            if [ -z "$base_gki" ]; then
                base_gki=$(echo "$current" | cut -d'-' -f1,2)
            fi
            
            # If the KMI number is missing (e.g. it was overwritten by '-OP'), inject a fallback
            if ! echo "$base_gki" | grep -qE -- '-android[0-9]+-[0-9]+'; then
                local android_ver=$(echo "$base_gki" | grep -oE -- '[0-9]+$')
                if [ "$android_ver" = "12" ]; then
                    base_gki="${base_gki}-9"
                elif [ "$android_ver" = "14" ]; then
                    base_gki="${base_gki}-11"
                else
                    base_gki="${base_gki}-9"
                fi
            fi
            
            # Generate a consistent fake 12-character commit hash
            local fake_hash="g$(echo "spoof_$current" | md5sum | cut -c1-12)"
            kver="${base_gki}-${fake_hash}"
        else
            kver="default"
        fi
    fi
    
    if [ "$kbuild" = "default" ] || [ -z "$kbuild" ]; then
        local rom_date=$(getprop ro.build.date 2>/dev/null)
        if [ -n "$rom_date" ]; then
            kbuild="#1 SMP PREEMPT $rom_date"
        else
            kbuild="default"
        fi
    fi
    
    susfs_run set_uname "$kver" "$kbuild"
}
cmd_status() {
    init_config
    local supported="false"
    check_susfs && supported="true"
    b() { [ "$(get_cfg "$1")" = "1" ] && echo true || echo false; }
    printf '{\n  "supported": %s,\n  "hide_sus_mnts_for_non_su_procs": %s,\n  "spoof_cmdline": %s,\n  "spoof_uname": %s,\n  "kernel_version": "%s",\n  "kernel_build": "%s",\n  "enable_avc_log_spoofing": %s,\n  "su_compat": %s,\n  "kernel_umount": %s,\n  "selinux_hide": %s,\n  "developer_options": %s,\n  "usb_debugging": %s,\n  "wireless_debugging": %s,\n  "selinux_enforcing": %s\n}\n' \
        "$supported" \
        "$(b hide_sus_mnts_for_non_su_procs)" \
        "$(b spoof_cmdline)" \
        "$(b spoof_uname)" \
        "$(get_cfg kernel_version)" \
        "$(get_cfg kernel_build)" \
        "$(b enable_avc_log_spoofing)" \
        "$(b su_compat)" \
        "$(b kernel_umount)" \
        "$(b selinux_hide)" \
        "$(b developer_options)" \
        "$(b usb_debugging)" \
        "$(b wireless_debugging)" \
        "$(b selinux_enforcing)"
}
cmd_toggle() {
    init_config
    local key="$1" value="$2"
    [ "$value" = "true" ] && value=1 || value=0
    set_cfg "$key" "$value"
    cmd_apply_post_fs_data >/dev/null 2>&1
    cmd_apply_boot_completed >/dev/null 2>&1
    echo "OK"
}
cmd_set_val() {
    init_config
    set_cfg "$1" "$2"
    cmd_apply_post_fs_data >/dev/null 2>&1
    cmd_apply_boot_completed >/dev/null 2>&1
    echo "OK"
}
cmd_get_rule() {
    cat "$PERSISTENT_DIR/$1" 2>/dev/null || echo ""
}
cmd_set_rule() {
    echo "$2" > "$PERSISTENT_DIR/$1"
    cmd_apply_post_fs_data >/dev/null 2>&1
    cmd_apply_boot_completed >/dev/null 2>&1
    echo "OK"
}
cmd_apply_post_fs_data() {
    init_config
    check_susfs || return 0
    if [ "$(get_cfg spoof_cmdline)" = "1" ]; then
        local FAKE_BOOTCONFIG="$PERSISTENT_DIR/fake_bootconfig"
        local FAKE_CMDLINE="$PERSISTENT_DIR/fake_cmdline"
        if [ -f /proc/bootconfig ]; then
            cat /proc/bootconfig > "$FAKE_BOOTCONFIG"
            sed -i 's/androidboot.warranty_bit = "1"/androidboot.warranty_bit = "0"/' "$FAKE_BOOTCONFIG" 2>/dev/null
            sed -i 's/androidboot.verifiedbootstate = "orange"/androidboot.verifiedbootstate = "green"/' "$FAKE_BOOTCONFIG" 2>/dev/null
            susfs_run set_cmdline_or_bootconfig "$FAKE_BOOTCONFIG"
        fi
        if [ -f /proc/cmdline ]; then
            cat /proc/cmdline > "$FAKE_CMDLINE"
            sed -i 's/androidboot.warranty_bit=1/androidboot.warranty_bit=0/' "$FAKE_CMDLINE" 2>/dev/null
            sed -i 's/androidboot.verifiedbootstate=orange/androidboot.verifiedbootstate=green/' "$FAKE_CMDLINE" 2>/dev/null
            susfs_run set_cmdline_or_bootconfig "$FAKE_CMDLINE"
        fi
    fi
    if [ "$(get_cfg enable_avc_log_spoofing)" = "1" ]; then
        susfs_run enable_avc_log_spoofing 1
    else
        susfs_run enable_avc_log_spoofing 0
    fi
    if [ "$(get_cfg hide_sus_mnts_for_non_su_procs)" = "1" ]; then
        susfs_run hide_sus_mnts_for_non_su_procs 1
    else
        susfs_run hide_sus_mnts_for_non_su_procs 0
    fi
    [ "$(get_cfg spoof_uname)" = "1" ] && spoof_uname_fn
    [ -f "$PERSISTENT_DIR/sus_path.txt" ] && \
        grep -v "^#" "$PERSISTENT_DIR/sus_path.txt" | while IFS= read -r i; do
            [ -n "$i" ] && susfs_run add_sus_path "$i"
        done
    [ -f "$PERSISTENT_DIR/sus_path_loop.txt" ] && \
        grep -v "^#" "$PERSISTENT_DIR/sus_path_loop.txt" | while IFS= read -r i; do
            [ -n "$i" ] && susfs_run add_sus_path_loop "$i"
        done
}
cmd_apply_boot_completed() {
    init_config
    check_susfs || return 0
    if [ -x "$KSU_BIN" ]; then
        [ "$(get_cfg su_compat)" = "1" ] && "$KSU_BIN" feature set su_compat 1 >/dev/null 2>&1
        [ "$(get_cfg kernel_umount)" = "1" ] && "$KSU_BIN" feature set kernel_umount 1 >/dev/null 2>&1
        [ "$(get_cfg selinux_hide)" = "1" ] && "$KSU_BIN" feature set selinux_hide 1 >/dev/null 2>&1
        "$KSU_BIN" feature save >/dev/null 2>&1
    fi
    if [ "$(get_cfg developer_options)" = "1" ]; then
        settings put global development_settings_enabled 1 2>/dev/null
    else
        settings put global development_settings_enabled 0 2>/dev/null
    fi
    if [ "$(get_cfg usb_debugging)" = "1" ]; then
        settings put global adb_enabled 1 2>/dev/null
    else
        settings put global adb_enabled 0 2>/dev/null
    fi
    if [ "$(get_cfg wireless_debugging)" = "1" ]; then
        settings put global adb_wifi_enabled 1 2>/dev/null
    else
        settings put global adb_wifi_enabled 0 2>/dev/null
    fi
    if [ "$(get_cfg selinux_enforcing)" = "1" ]; then
        [ "$(getenforce 2>/dev/null)" != "Enforcing" ] && setenforce 1 2>/dev/null
    fi
    if [ -f "$PERSISTENT_DIR/sus_kstat_statically.json" ]; then
        awk '/^[[:space:]]*\{/,/^[[:space:]]*\}/' "$PERSISTENT_DIR/sus_kstat_statically.json" | {
            current_obj=""
            while IFS= read -r line; do
                if echo "$line" | grep -q '^[[:space:]]*{'; then current_obj=""; fi
                current_obj="$current_obj $line"
                if echo "$line" | grep -q '^[[:space:]]*}'; then
                    IFS='	' read -r path ino dev nlink size atime atime_nsec mtime mtime_nsec ctime ctime_nsec blocks blksize <<EOF
$(echo "$current_obj" | awk '
                    {
                        while (match($0, /"[a-z_]+"[[:space:]]*:[[:space:]]*"[^"]*"/)) {
                            pair = substr($0, RSTART, RLENGTH)
                            $0 = substr($0, RSTART + RLENGTH)
                            k = pair; sub(/"[[:space:]]*:.*/, "", k); sub(/^"/, "", k)
                            v = pair; sub(/^[^:]*:[[:space:]]*"/, "", v); sub(/"$/, "", v)
                            if (!(k in seen)) { seen[k] = 1; val[k] = v }
                        }
                    }
                    END {
                        printf "%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\n", \
                            val["path"], val["ino"], val["dev"], val["nlink"], val["size"], \
                            val["atime"], val["atime_nsec"], val["mtime"], val["mtime_nsec"], \
                            val["ctime"], val["ctime_nsec"], val["blocks"], val["blksize"]
                    }')
EOF
                    if [ -n "$path" ]; then
                        susfs_run add_sus_kstat_statically "$path" "$ino" "$dev" "$nlink" "$size" "$atime" "$atime_nsec" "$mtime" "$mtime_nsec" "$ctime" "$ctime_nsec" "$blocks" "$blksize"
                    fi
                    current_obj=""
                fi
            done
        } 2>/dev/null || true
    fi
    [ -f "$PERSISTENT_DIR/sus_maps.txt" ] && \
        grep -v "^#" "$PERSISTENT_DIR/sus_maps.txt" | while IFS= read -r i; do
            [ -n "$i" ] && susfs_run add_sus_map "$i"
        done
}
CMD="${1:-status}"
shift 2>/dev/null
case "$CMD" in
    status)               cmd_status ;;
    toggle)               cmd_toggle "$@" ;;
    set_val)              cmd_set_val "$@" ;;
    get_rule)             cmd_get_rule "$@" ;;
    set_rule)             cmd_set_rule "$@" ;;
    apply_post_fs_data)   cmd_apply_post_fs_data ;;
    apply_boot_completed) cmd_apply_boot_completed ;;
    *) die "Unknown command: $CMD" ;;
esac
