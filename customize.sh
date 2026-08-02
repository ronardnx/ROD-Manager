ui_print "- Installing ROD-Manager components"
set_perm_recursive "$MODPATH/webroot" 0 0 0755 0644
set_perm "$MODPATH/webroot/susfs" 0 0 0755
ui_print "- Permissions set successfully"
ui_print "- Open KernelSU Manager → Modules → ROD Manager → WebUI"
