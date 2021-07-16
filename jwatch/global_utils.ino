
void setup_watch() {
  Serial.begin(115200);

  esp_log("\n     _  ____      ____      _          __       ");
  esp_log("    (_)|_  _|    |_  _|    / |_       [  |      ");
  esp_log("    __   \\ \\  /\\  / /,--. `| |-'.---.  | |--.   ");
  esp_log("   [  |   \\ \\/  \\/ /`'_\\ : | | / /'`\\] | .-. |  ");
  esp_log(" _  | |    \\  /\\  / // | |,| |,| \\__.  | | | |  ");
  esp_log("[ \\_| |     \\/  \\/  \\'-;__/\\__/'.___.'[___]|__] ");
  esp_log(" \\____/                                         ");
  esp_log("");
  
  ttgo = TTGOClass::getWatch();
  ttgo->begin();
  ttgo->openBL();
  set_bl(DEFAULT_BL_VALUE);
  ttgo->lvgl_begin();

  rtc = ttgo->rtc;
  rtc->check();

  if (USE_ACCELEROMETER) { setup_bma(); }
  
  pinMode(AXP202_INT, INPUT_PULLUP);
  attachInterrupt(AXP202_INT, [] {
      irq_axp = true;
  }, FALLING);

  ttgo->power->setPowerOutPut(AXP202_EXTEN, AXP202_OFF);
  ttgo->power->setPowerOutPut(AXP202_DCDC2, AXP202_OFF);
  ttgo->power->setPowerOutPut(AXP202_LDO3, AXP202_OFF);
  ttgo->power->setPowerOutPut(AXP202_LDO4, AXP202_OFF);

  ttgo->power->adc1Enable(AXP202_VBUS_VOL_ADC1 | AXP202_VBUS_CUR_ADC1 | AXP202_BATT_CUR_ADC1 | AXP202_BATT_VOL_ADC1, true);
  ttgo->power->enableIRQ(AXP202_PEK_SHORTPRESS_IRQ, true);
  ttgo->power->clearIRQ();

  setup_styles();

  notifications.begin("jwatch");
  notifications.setConnectionStateChangedCallback(onBLEStateChanged);
  notifications.setNotificationCallback(onNotificationArrived);
  notifications.setRemovedCallback(onNotificationRemoved);

  lv_task_t * update_task = lv_task_create(update_screens_task, 1000, LV_TASK_PRIO_MID, NULL);
  lv_task_t * irq_task = lv_task_create(update_irq_task, 100, LV_TASK_PRIO_MID, NULL);
  lv_task_t * device_timeout_task = lv_task_create(update_device_timeout_task, 100, LV_TASK_PRIO_MID, NULL);

  lv_obj_set_style_local_bg_color(lv_scr_act(), LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLACK);

  esp_log("\nwatch initialized");

  if(!SPIFFS.begin(false)){
    esp_log("SPIFFS Mount Failed");
  }

  read_file_to_wifi_pair(SPIFFS);

  xTaskCreate(update_wifi_task, "update_wifi", 2048, NULL, 2, NULL);
  xTaskCreate(update_battery_percentage_history_task, "battery_percentage_history", 2048, NULL, 2, NULL);
}

void esp_log(String string) {
  Serial.println(string);
}

void set_bl(uint8_t v) {
  ttgo->bl->adjust(map(v*v, 0, 255*255, 0, 255));
  last_bl_value = v;
}
