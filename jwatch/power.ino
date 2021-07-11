

void button_short_press() {
  if (active) {
    switch (current_screen) {
      
      case BLANK_SCREEN: {
        switch_screen_fade(CLOCK_SCREEN);
        break;
      }
  
      case CLOCK_SCREEN: {
        power_off();
        break;
      }

      default: {
        switch_screen_fade(CLOCK_SCREEN);
        break;
      }
  
//      case MENU_SCREEN: {
//        switch_screen_fade(CLOCK_SCREEN);
//        break;
//      }
//  
//      case SETTINGS_SCREEN: {
//        switch_screen_fade(MENU_SCREEN);
//        break;
//      }
//  
//      case BATTERY_SCREEN: {
//        switch_screen_fade(SETTINGS_SCREEN);
//        break;
//      }
//  
//      case WATCH_FACE_SCREEN: {
//        switch_screen_fade(SETTINGS_SCREEN);
//        break;
//      }
//  
//      case WIFI_SCREEN: {
//        switch_screen_fade(SETTINGS_SCREEN);
//        break;
//      }
//  
//      case APPS_SCREEN: {
//        switch_screen_fade(MENU_SCREEN);
//        break;
//      }
//
//      case APP_PRINTER_SCREEN: {
//        switch_screen_fade(APPS_SCREEN);
//        break;
//      }
      
    }
  } else {
    power_on();
  }
}

void power_on () {
  esp_log("powering on");

  setCpuFrequencyMhz(HIGH_CPU);
  
  ttgo->startLvglTick();
  ttgo->displayWakeup();
  ttgo->rtc->syncToSystem();
  lv_disp_trig_activity(NULL);
  update_screens_task(nullptr);
  ttgo->openBL();

  wifi_active = wifi_active_temp;
  update_wifi();
  
  active = true;
}

void power_off () {
  esp_log("powering off");

  ttgo->closeBL();
  ttgo->stopLvglTick();
  ttgo->displaySleep();
  
  wifi_active_temp = wifi_active; wifi_active = false;
  update_wifi();
  
  setCpuFrequencyMhz(DEAD_CPU);
  
  active = false;
}

void update_irq() {
  if (irq_axp) {
    irq_axp = false;
    ttgo->power->readIRQ();
    if (ttgo->power->isPEKShortPressIRQ()) {
      button_short_press();
    }
    ttgo->power->clearIRQ();
  }

  if (irq_bma) {
    bool wait = false;
    while (!wait) {
      wait = ttgo->bma->readInterrupt();
    }
    if (ttgo->bma->isStepCounter()) {
      step_count = ttgo->bma->getCounter();
      esp_log("updated step count to "); Serial.print(step_count);
    }
    if (ttgo->bma->isTilt() && !active) {
      power_on();
    }
  }
}
