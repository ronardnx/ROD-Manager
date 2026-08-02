#!/system/bin/sh
MODDIR="${0%/*}"
chmod +x "$MODDIR/webroot/"*.sh 2>/dev/null
chmod +x "$MODDIR/webroot/susfs" 2>/dev/null
if [ -f "$MODDIR/webroot/susfs.sh" ]; then
    sh "$MODDIR/webroot/susfs.sh" apply_post_fs_data &
fi
exit 0
