
void update_screens_task(lv_task_t * task) {

  switch (current_screen) {
    case CLOCK_SCREEN: {
      update_screen_time(lv_scr_act());
      break;
    }

    case BATTERY_SCREEN: {
      update_battery_label();
      break;
    }

    case WIFI_SCREEN: {
      if (!use_wifi) { lv_label_set_text(wifi_status_label, "WiFi Disabled"); return; }
      if (wifi_active && !wifi_active_last) {
        lv_label_set_text(wifi_status_label, "Starting WiFi");
      } else if (!wifi_active && wifi_active_last) {
        lv_label_set_text(wifi_status_label, "Stopping WiFi");
      } else if (wifi_active) {
        if (WiFi.isConnected()) {
          lv_label_set_text(wifi_status_label, ("WiFi Connected (" + WiFi.SSID() + ")").c_str());
        } else {
          lv_label_set_text(wifi_status_label, "Attempting to connect...");
        }
      } else {
        lv_label_set_text(wifi_status_label, "WiFi Disabled");
      }
      break;
    }
  }
}

void update_irq_task(lv_task_t * task) {
  update_irq();
}

void update_device_timeout_task(lv_task_t * task) {
  if (lv_disp_get_inactive_time(NULL) > DEVICE_TIMEOUT) {
    power_off();
  }
}

void update_device_time_task(lv_task_t * task) {
  update_rtc_time();
}

void update_low_cpu_task(lv_task_t * task) {
  setCpuFrequencyMhz(LOW_CPU);
}

void update_wifi_task(void *pvParameters) {
  delay(5000);
  setup_wifi_events();
  
  while (true) {
    if (use_wifi) { update_wifi(); }
    delay(3000);
  }
}

void update_battery_percentage_history_task(void *pvParameters){
  for (int i = 0; i < BATTERY_PERCENTAGE_HISTORY_READINGS; i++) {
    battery_percentage_history[i] = 0;  
  }

  while (true) {
    uint8_t current = ttgo->power->getBattPercentage();
//    uint8_t current = 50;
    
    if (battery_percentage_history_readings < BATTERY_PERCENTAGE_HISTORY_READINGS) {
      battery_percentage_history[battery_percentage_history_readings] = current;
      battery_percentage_history_readings++;
    } else {
      for (int i = 0; i < BATTERY_PERCENTAGE_HISTORY_READINGS - 1; i++) {
        battery_percentage_history[i] = battery_percentage_history[i+1];
      }
      battery_percentage_history[BATTERY_PERCENTAGE_HISTORY_READINGS] = current;
    }

    delay(BATTERY_PERCENTAGE_HISTORY_HOURS * 60 * 60 * 1000 / BATTERY_PERCENTAGE_HISTORY_READINGS);
  }
}
