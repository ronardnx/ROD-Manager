#!/system/bin/sh
MODDIR="${0%/*}"
until [ "$(getprop sys.boot_completed)" = "1" ]; do
    sleep 2
done
resetprop persist.sys.usb.config mtp
mkdir -p /data/adb/rod
date '+%Y-%m-%dT%H:%M:%S' > /data/adb/rod/last_boot
"$MODDIR/webroot/rodd" susfs apply_boot_completed &
exit 0
