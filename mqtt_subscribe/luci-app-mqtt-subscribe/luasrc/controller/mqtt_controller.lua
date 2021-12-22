module("luci.controller.mqtt_controller", package.seeall)

function index()
    entry( { "admin", "services", "mqtt", "subscriber" }, cbi("mqtt_model"), _("Subscriber"), 100).leaf = true
end