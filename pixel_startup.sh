#!/system/bin

/system/bin/settings put global charging_sounds_enabled 1
/system/bin/settings put global settings_about_phone_v2 true
/system/bin/settings put global settings_bluetooth_while_driving true
/system/bin/settings put global settings_zone_picker_v2 true
/system/bin/settings put global settings_systemui_theme true
/system/bin/settings put system allow_screen_shot 1

is_enabled_dev_opt=$(/system/bin/settings get global development_settings_enabled)

if [ "$is_enabled_dev_opt" = "1" ]; then
    /system/bin/appops set com.android.chrome REQUEST_INSTALL_PACKAGES allow
fi
