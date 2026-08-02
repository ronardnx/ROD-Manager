#!/system/bin/sh
PIF_DIR="/data/adb/modules/playintegrityfix"
PIF_ACTION="$PIF_DIR/autopif.sh"
die() { echo "ERROR: $*" >&2; exit 1; }
cmd_status() {
    local installed="false"
    [ -d "$PIF_DIR" ] && installed="true"
    local version=""
    [ -f "$PIF_DIR/module.prop" ] && \
        version=$(grep '^version=' "$PIF_DIR/module.prop" | sed 's/version=//')
    local last_fetch=""
    [ -f /data/adb/rod/pif_last_fetch ] && last_fetch=$(cat /data/adb/rod/pif_last_fetch)
    echo "{"
    echo "  \"installed\": $installed,"
    echo "  \"version\": \"$version\","
    echo "  \"last_fetch\": \"$last_fetch\""
    echo "}"
}
cmd_fetch() {
    [ -f "$PIF_ACTION" ] || die "autopif.sh not found at $PIF_ACTION"
    sh "$PIF_ACTION" >/dev/null 2>&1
    local exit_code=$?
    if [ $exit_code -eq 0 ]; then
        mkdir -p /data/adb/rod
        date +%s > /data/adb/rod/pif_last_fetch
    fi
    exit $exit_code
}
CMD="${1:-status}"
shift 2>/dev/null
case "$CMD" in
    status)      cmd_status "$@" ;;
    fetch)       cmd_fetch "$@" ;;
    *) die "Unknown command: $CMD" ;;
esac
