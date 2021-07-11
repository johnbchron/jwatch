#include <ArduinoJson.h>

//#include <WiFiUdp.h>
//#include <WiFiClient.h>
//#include <WiFiServer.h>
//#include <WiFiSTA.h>
//#include <WiFiType.h>
//#include <WiFiAP.h>
#include <WiFi.h>
#include <WiFiMulti.h>
//#include <WiFiScan.h>
//#include <WiFiGeneric.h>
//#include <ETH.h>

#include "FS.h"
#include "SPIFFS.h"

#include "config.h"
#include "esp32notifications.h"
#include <TimeLib.h>

LV_IMG_DECLARE(background_1);

enum screens {BLANK_SCREEN, CLOCK_SCREEN, MENU_SCREEN, SETTINGS_SCREEN, WATCH_FACE_SCREEN, BATTERY_SCREEN, WIFI_SCREEN, APPS_SCREEN, APP_PRINTER_SCREEN};
enum watch_faces {MINIMALIST_DIGITAL_FACE, ANALOG_FACE, BG_1_FACE};

TTGOClass * ttgo;
PCF8563_Class * rtc;
BLENotifications notifications;
WiFiMulti wifi_multi;

uint8_t current_screen = BLANK_SCREEN;
uint8_t current_watch_face = ANALOG_FACE;
lv_obj_t * prepared_screen;

static lv_style_t label_style;
static lv_style_t big_label_style;
static lv_style_t small_label_style;
static lv_style_t giant_label_style;
static lv_style_t button_style;
static lv_style_t switch_style;
static lv_style_t page_style;
static lv_style_t inner_page_style;
static lv_style_t analog_clock_face_gauge_style;
static lv_style_t analog_clock_face_gauge_major_line_style;
static lv_style_t analog_clock_face_gauge_minor_line_style;
static lv_style_t analog_clock_face_gauge_null_style;

lv_obj_t * notif_messagebox = NULL;
lv_obj_t * battery_label_reference;
lv_obj_t * analog_clock_main_gauge;
lv_obj_t * analog_clock_second_gauge;
lv_obj_t * wifi_status_label;
lv_obj_t * wifi_toggle;
lv_obj_t * printer_status_label;
String notif_messagebox_title;
String notif_messagebox_message;
bool notif_messagebox_exists = false;
bool irq_axp = false;
bool irq_bma = false;
bool active = true;
bool wifi_active = true;
bool wifi_active_last = false;
bool wifi_active_temp = true;
int step_count = 0;
bool use_transitions = true;
bool use_wifi = true;
uint8_t wifi_failed_connection_attempts = 0;

uint8_t last_bl_value = DEFAULT_BL_VALUE;
//timelib_t timelib_now;

void onBLEStateChanged(BLENotifications::State state);
void onNotificationArrived(const ArduinoNotification * notification, const Notification * rawNotificationData);
void onNotificationRemoved(const ArduinoNotification * notification, const Notification * rawNotificationData);

void refresh_printer_status(lv_obj_t * obj, lv_event_t event);
String refresh_printer_status();

void update_battery_label();

void setup_watch();
void esp_log(String string);

void setup_bma();

void switch_screen(uint8_t _screen);
void switch_screen_fade(uint8_t _screen);
void prepare_screen(uint8_t _screen);

void read_file_to_wifi_pair(fs::FS &fs);

void setup_styles();
void setup_standard_button(lv_obj_t * btn);

void button_short_press();
void power_on();
void power_off();
void update_irq();

void update_screens_task(lv_task_t * task);
void update_device_time_task(lv_task_t * task);
void update_low_cpu_task(lv_task_t * task);
void update_wifi_task(void *pvParameters);

void update_rtc_time();
void update_screen_time();
String format_datetime_12_hour(RTC_Date _now_datetime);
String format_datetime_12_hour_with_newline(RTC_Date _now_datetime);
String format_datetime_24_hour(RTC_Date _now_datetime);
String format_datetime_12_hour_with_seconds(RTC_Date _now_datetime);
String format_datetime_24_hour_with_seconds(RTC_Date _now_datetime);
String zero_pad(int value);

void update_wifi();
void setup_wifi_events();

void setup() {
  setup_watch();
  
  switch_screen_fade(CLOCK_SCREEN);
}

void loop() {
  if (active) {
    lv_task_handler();
  } else {
    update_irq();
  }
  delay(10);
}
