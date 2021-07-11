
void update_battery_label(){
  float vbus_v = ttgo->power->getVbusVoltage();
  float vbus_c = ttgo->power->getVbusCurrent();
  float batt_v = ttgo->power->getBattVoltage();
  uint8_t per  = ttgo->power->getBattPercentage();

  String msg;

  msg = "Battery " + String(per) + "% charged";

  if (ttgo->power->isVBUSPlug()) {
    msg = msg + "\n\nMicro USB connected";
  } else {
    msg = msg + "\n\nMicro USB disconnected";
  }

  if (ttgo->power->isChargeing()) {
    if (ttgo->power->getBattChargeCurrent() < 10) {
      msg = msg + "\nStable charge";
    } else {
      msg = msg + "\nCharging at #00ff00 +" + String(ttgo->power->getBattChargeCurrent()) + "# mA";
    }
  } else {
    if (ttgo->power->getBattDischargeCurrent() < 10) {
      msg = msg + "\nStable charge";
    } else {
      msg = msg + "\nDischarging at #ff0000 -" + String(ttgo->power->getBattDischargeCurrent()) + "# mA";
    }
  }

  msg = msg + "\n\nbattery voltage = " + batt_v/1000 + " V";
  msg = msg + "\nbus voltage = " + vbus_v/1000 + " V";
  msg = msg + "\nbus current = " + vbus_c + " mA";

  lv_label_set_text(battery_label_reference, msg.c_str());
}
