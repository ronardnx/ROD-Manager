#!/system/bin/sh
MODDIR="${0%/*}"
chmod +x "$MODDIR/webroot/rodd" 2>/dev/null
# Apply susfs post-fs-data rules directly
"$MODDIR/webroot/rodd" susfs apply_post_fs_data
exit 0
