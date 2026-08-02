#!/system/bin/sh
PROP="/data/adb/modules/zygisksu/module.prop"
ZYGISKD=""
if [ -f "$PROP" ]; then
  if [ -f "/data/adb/modules/zygisksu/bin/zygiskd" ]; then
    ZYGISKD="/data/adb/modules/zygisksu/bin/zygiskd"
  elif [ -f "/data/adb/modules/zygisksu/bin/zygiskd64" ]; then
    ZYGISKD="/data/adb/modules/zygisksu/bin/zygiskd64"
  fi
fi
STATE_DIR="/data/adb/rod"

case "$1" in
  status)
    if [ ! -f "$PROP" ]; then echo '{"installed":false}'; exit 0; fi
    ver=$(grep "^version=" "$PROP" | cut -d'=' -f2 | tr -d '\r')
    state="Not configured yet"
    [ -f "$STATE_DIR/zn_state" ] && state=$(cat "$STATE_DIR/zn_state")
    last=""
    [ -f "$STATE_DIR/zn_last_apply" ] && last=$(cat "$STATE_DIR/zn_last_apply")
    echo "{\"installed\":true,\"version\":\"$ver\",\"state\":\"$state\",\"last_apply\":\"$last\"}"
    ;;
  apply)
    "$ZYGISKD" enforce-denylist just_umount
    "$ZYGISKD" memory-type anonymous
    "$ZYGISKD" linker builtin
    mkdir -p "$STATE_DIR"
    echo "Denylist: umount | Memory: anon | Linker: builtin" > "$STATE_DIR/zn_state"
    date +%s > "$STATE_DIR/zn_last_apply"
    ;;
esac
