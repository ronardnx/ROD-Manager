#!/system/bin/sh
HMA_PKG="org.frknkrc44.hma_oss"
APP_PRESETS='["accessibility_apps","custom_rom","detector_apps","root_apps","shizuku_dhizuku","sus_apps","xposed"]'
SETTINGS_PRESETS='["accessibility","dev_options","input_method"]'
die() { echo "ERROR: $*" >&2; exit 1; }
find_hma_dir() {
    for d in /data/misc/hide_my_applist_*; do
        [ -d "$d" ] && echo "$d" && return
    done
}
cmd_status() {
    local hma_dir
    hma_dir=$(find_hma_dir)
    local service_running="false"
    [ -n "$hma_dir" ] && service_running="true"
    local last_apply=""
    [ -f /data/adb/rod/hma_last_apply ] && last_apply=$(cat /data/adb/rod/hma_last_apply)
    echo "{"
    echo "  \"module_installed\": $service_running,"
    echo "  \"service_dir\": \"${hma_dir:-not found}\","
    echo "  \"last_apply\": \"$last_apply\""
    echo "}"
}
cmd_apply_presets() {
    local hma_dir
    hma_dir=$(find_hma_dir)
    [ -z "$hma_dir" ] && die "HMA-OSS service dir not found at /data/misc/hide_my_applist_*. Is HMA-OSS running?"
    local config_file="$hma_dir/config.json"
    local config_version=3
    if [ -f "$config_file" ]; then
        config_version=$(grep -o '"configVersion":[0-9]*' "$config_file" | head -1 | grep -o '[0-9]*')
        [ -z "$config_version" ] && config_version=3
    fi
    local existing_scope=""
    if [ -f "$config_file" ]; then
        existing_scope=$(grep -o '"scope":{[^}]*}' "$config_file" 2>/dev/null || true)
    fi
    local apps
    apps=$(pm list packages -3 2>/dev/null | sed 's/^package://' | tr -d '\r' | grep -v '^$')
    if [ -z "$apps" ]; then
        apps=$(ls /data/app 2>/dev/null | sed 's/-[^-]*$//' | grep '\.' | sort -u)
    fi
    local EXCLUDE_APPS="${HMA_PKG}
io.github.a13e300.ksuwebui
me.weishu.kernelsu
com.topjohnwu.magisk
com.google.android.gms
com.google.android.gsf
com.android.vending
android"
    if [ -f /data/adb/rod/hma_exclude.txt ]; then
        EXCLUDE_APPS="${EXCLUDE_APPS}
$(cat /data/adb/rod/hma_exclude.txt)"
    fi
    apps=$(echo "$apps" | while read -r pkg; do
        [ -z "$pkg" ] && continue
        echo "$EXCLUDE_APPS" | grep -qxF "$pkg" || echo "$pkg"
    done)
    local tmp_json="/data/local/tmp/hma_cfg_$$.json"
    local scope_entries=""
    local first=1
    for pkg in $apps; do
        [ -z "$pkg" ] && continue
        local entry
        entry="    \"$pkg\": {
      \"useWhitelist\": false,
      \"excludeSystemApps\": true,
      \"hideInstallationSource\": true,
      \"hideSystemInstallationSource\": true,
      \"excludeTargetInstallationSource\": false,
      \"invertActivityLaunchProtection\": false,
      \"excludeVoldIsolation\": false,
      \"restrictedZygotePermissions\": [],
      \"applyTemplates\": [],
      \"applyPresets\": $APP_PRESETS,
      \"applySettingTemplates\": [],
      \"applySettingsPresets\": $SETTINGS_PRESETS,
      \"extraAppList\": [],
      \"extraOppositeAppList\": []
    }"
        if [ $first -eq 1 ]; then
            scope_entries="$entry"
            first=0
        else
            scope_entries="$scope_entries,
$entry"
        fi
    done
    cat > "$tmp_json" <<EOF
{
  "configVersion": $config_version,
  "detailLog": false,
  "errorOnlyLog": false,
  "maxLogSize": 512,
  "forceMountData": true,
  "disableActivityLaunchProtection": false,
  "altAppDataIsolation": false,
  "altVoldAppDataIsolation": false,
  "skipSystemAppDataIsolation": true,
  "packageQueryWorkaround": false,
  "webViewProtection": true,
  "defaultConfig": null,
  "templates": {},
  "settingsTemplates": {},
  "disabledHooks": [],
  "scope": {
$scope_entries
  }
}
EOF
    cp "$tmp_json" "$config_file" || die "Failed to write to $config_file"
    chmod 600 "$config_file"
    rm -f "$tmp_json"
    mkdir -p /data/adb/rod
    date +%s > /data/adb/rod/hma_last_apply
    am force-stop "$HMA_PKG" >/dev/null 2>&1 || true
}
cmd_get_rule() {
    [ -f "/data/adb/rod/$1" ] && cat "/data/adb/rod/$1" || true
}
cmd_set_rule() {
    mkdir -p /data/adb/rod
    echo "$2" > "/data/adb/rod/$1"
    echo "OK"
}
cmd_get_packages() {
    pm list packages -3 | sed 's/^package://' | tr -d '\r' || true
}
cmd_get_all_labels() {
    CLASSPATH=/data/adb/modules/rod/webroot/AppLabelResolver.dex app_process /system/bin AppLabelResolver 2>/dev/null || true
}
CMD="${1:-status}"
shift 2>/dev/null
case "$CMD" in
    status)        cmd_status "$@" ;;
    apply_presets) cmd_apply_presets "$@" ;;
    get_rule)      cmd_get_rule "$@" ;;
    set_rule)      cmd_set_rule "$@" ;;
    get_packages)   cmd_get_packages "$@" ;;
    get_all_labels) cmd_get_all_labels "$@" ;;
    *) die "Unknown command: $CMD" ;;
esac
