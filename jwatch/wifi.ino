

void update_wifi() {
//  ttgo->disableTouchIRQ();
  
  if (wifi_active && !wifi_active_last) {
    WiFi.mode(WIFI_STA);
    WiFi.setHostname(DEVICE_NAME);
  } else if (!wifi_active && wifi_active_last) {
    WiFi.mode(WIFI_OFF);    
  } else if (wifi_active && wifi_active_last) {
    if (!WiFi.isConnected()) {
      if (wifi_failed_connection_attempts < 3) {
        if (wifi_multi.run() != WL_CONNECTED) {
          wifi_failed_connection_attempts += 1;
        } else {
          update_wifi();
        }
      } else {
        wifi_active = false;
        if (current_screen == WIFI_SCREEN) { lv_switch_off(wifi_toggle, LV_ANIM_ON); }
        wifi_failed_connection_attempts = 0;
        update_wifi();
      }
    }
  } else { }
  wifi_active_last = wifi_active;

//  ttgo->enableTouchIRQ();
}

void setup_wifi_events() {
  WiFi.onEvent([](WiFiEvent_t event, WiFiEventInfo_t info) {
    esp_log("wifi connected to " + WiFi.SSID());
  }, WiFiEvent_t::SYSTEM_EVENT_STA_CONNECTED);

  WiFi.onEvent([](WiFiEvent_t event, WiFiEventInfo_t info) {
    esp_log("wifi got ip " + WiFi.localIP().toString());
  }, WiFiEvent_t::SYSTEM_EVENT_STA_GOT_IP);

  WiFi.onEvent([](WiFiEvent_t event, WiFiEventInfo_t info) {
    esp_log("wifi disconnnected");
  }, WiFiEvent_t::SYSTEM_EVENT_STA_DISCONNECTED);
}
