#!/system/bin/sh
MODDIR="${0%/*}"
chmod +x "$MODDIR/webroot/rodd" 2>/dev/null

mkdir -p /data/adb/rod
# Backup the real service binary so we can call it without hitting the redirect
cp /system/bin/service /data/adb/rod/service_real
chmod 755 /data/adb/rod/service_real

# Create the wrapper script that filters out lineage
cat << 'EOF' > /data/adb/rod/service_fake.sh
#!/system/bin/sh
if [ "$1" = "check" ]; then
    case "$2" in
        profile|lineage*|vendor.lineage*)
            echo "Service $2: not found"
            exit 0
            ;;
    esac
fi
if [ "$1" = "list" ] || [ "$1" = "check" ]; then
    /data/adb/rod/service_real "$@" | grep -vi -E "lineage|profile"
else
    exec /data/adb/rod/service_real "$@"
fi
EOF
chmod 755 /data/adb/rod/service_fake.sh

# Apply susfs post-fs-data rules directly
"$MODDIR/webroot/rodd" susfs apply_post_fs_data

# Hide all lineage init scripts and binaries completely from Duck Detector
# SUSFS cannot fake-rename files in directory listings, so we must hide them.
for file in /vendor/etc/init/*lineage*.rc; do
    if [ -f "$file" ]; then
        "$MODDIR/webroot/rodd" susfs add_sus_path "$file"
    fi
done

for file in /vendor/bin/hw/*lineage*; do
    if [ -f "$file" ]; then
        "$MODDIR/webroot/rodd" susfs add_sus_path "$file"
    fi
done
# Hide LineageOS overlay directories and idmap entries
for dir in /product/overlay/*[Ll]ineage* /system/overlay/*[Ll]ineage* /vendor/overlay/*[Ll]ineage*; do
    if [ -d "$dir" ] || [ -f "$dir" ]; then
        "$MODDIR/webroot/rodd" susfs add_sus_path "$dir"
    fi
done

# Hide idmap cache entries that reference lineage overlays
for f in /data/resource-cache/*[Ll]ineage*; do
    if [ -f "$f" ]; then
        "$MODDIR/webroot/rodd" susfs add_sus_path "$f"
    fi
done

exit 0
