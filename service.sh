#!/system/bin/sh
MODDIR="${0%/*}"
until [ "$(getprop sys.boot_completed)" = "1" ]; do
    sleep 2
done
resetprop persist.sys.usb.config mtp
mkdir -p /data/adb/rod
date '+%Y-%m-%dT%H:%M:%S' > /data/adb/rod/last_boot
if [ -f "$MODDIR/webroot/susfs.sh" ]; then
    sh "$MODDIR/webroot/susfs.sh" apply_boot_completed &
fi
sleep 15
if [ -f "$MODDIR/webroot/omk.sh" ]; then
    sh "$MODDIR/webroot/omk.sh" sync_denylist
fi
exit 0
