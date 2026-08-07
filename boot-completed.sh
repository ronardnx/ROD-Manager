#!/system/bin/sh
MODDIR="${0%/*}"
"$MODDIR/webroot/rodd" susfs boot_completed
exit 0
