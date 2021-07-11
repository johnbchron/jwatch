
void update_rtc_time() {
  time_t timelib_now = now();
  Serial.printf("setting watch rtc to %04d-%02d-%02d %02d:%02d:%02d\n", year(timelib_now), month(timelib_now), day(timelib_now), hour(timelib_now), minute(timelib_now), second(timelib_now));
  ttgo->rtc->setDateTime(year(timelib_now), month(timelib_now), day(timelib_now), hour(timelib_now), minute(timelib_now), second(timelib_now));
}

void update_screen_time(lv_obj_t * _scr) {
  RTC_Date now_datetime = rtc->getDateTime();

  switch (current_watch_face) {
  
    case MINIMALIST_DIGITAL_FACE: {
      lv_obj_t * clock_label = lv_obj_get_child(_scr, NULL);
      lv_label_set_text(clock_label, format_datetime_12_hour(now_datetime).c_str());
      
      break;
    }
  
    case ANALOG_FACE: {
      lv_gauge_set_value(analog_clock_main_gauge, 0, now_datetime.hour % 12);
      lv_gauge_set_value(analog_clock_second_gauge, 0, now_datetime.minute);
      lv_gauge_set_value(analog_clock_second_gauge, 1, now_datetime.second);
      
      break;
    }

    case BG_1_FACE: {
      lv_obj_t * clock_label = lv_obj_get_child(_scr, NULL);
      lv_label_set_text(clock_label, format_datetime_12_hour(now_datetime).c_str());
      
      break;
    }
  }
}

String format_datetime_12_hour(RTC_Date _now_datetime) {
  String now_string;
  if(_now_datetime.hour % 12 == 0) { now_string = "12"; } else { now_string = (_now_datetime.hour % 12); }
  now_string = now_string + F(":") + zero_pad(_now_datetime.minute);
  return now_string;
}

String format_datetime_12_hour_with_newline(RTC_Date _now_datetime) {
  String now_string;
  if(_now_datetime.hour % 12 == 0) { now_string = "12"; } else { now_string = (_now_datetime.hour % 12); }
  now_string = now_string + F("\n") + zero_pad(_now_datetime.minute);
  return now_string;
}

String format_datetime_24_hour(RTC_Date _now_datetime) {
  String now_string;
  now_string = zero_pad(_now_datetime.hour) + F(":") + zero_pad(_now_datetime.minute);
  return now_string;
}

String format_datetime_12_hour_with_seconds(RTC_Date _now_datetime) {
  String now_string;
  if(_now_datetime.hour % 12 == 0) { now_string = "12"; } else { now_string = zero_pad(_now_datetime.hour % 12); }
  now_string = now_string + F(":") + zero_pad(_now_datetime.minute) + F(":") + zero_pad(_now_datetime.second);
  return now_string;
}

String format_datetime_24_hour_with_seconds(RTC_Date _now_datetime) {
  String now_string;
  now_string = zero_pad(_now_datetime.hour) + F(":") + zero_pad(_now_datetime.minute) + F(":") + zero_pad(_now_datetime.second);
  return now_string;
}

String zero_pad(int value) {
  if (value < 10) {
    return '0' + String(value);
  } else {
    return String(value);
  }
}
