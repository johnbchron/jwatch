#include <TimeLib.h>

void onBLEStateChanged(BLENotifications::State state) {
  switch(state) {
    case BLENotifications::StateConnected: {
      esp_log("StateConnected - connected to a phone or tablet");
      
      lv_task_t * update_rtc_task = lv_task_create(update_device_time_task, 10000, LV_TASK_PRIO_MID, NULL);
      lv_task_once(update_rtc_task);
      break;
    }

    case BLENotifications::StateDisconnected: {
      esp_log("StateDisconnected - disconnected from a phone or tablet"); 
      /* We need to startAdvertising on disconnection, otherwise the ESP 32 will now be invisible.
      IMO it would make sense to put this in the library to happen automatically, but some people in the Espressif forums
      were requesting that they would like manual control over re-advertising.*/
      notifications.startAdvertising(); 
      break;
    }
  }
}

void onNotificationArrived(const ArduinoNotification * notification, const Notification * rawNotificationData) {
//  Serial.print("Got notification: ");   
//  Serial.println(notification->title); // The title, ie name of who sent the message
//  Serial.println(notification->message); // The detail, ie "be home for dinner at 7".
//  Serial.println(notification->type);  // Which app sent it
//  Serial.println(notifications.getNotificationCategoryDescription(notification->category));
//  Serial.println(notification->categoryCount);
  notif_messagebox_title = (notification->title);
  notif_messagebox_message = (notification->message);
//  create_notif_messagebox(notif_messagebox_title, notif_messagebox_message);
//  esp_log("new notification: " + notif_messagebox_title + " | " + notif_messagebox_message);
}

void onNotificationRemoved(const ArduinoNotification * notification, const Notification * rawNotificationData) {
//   Serial.print("Removed notification: ");   
//   Serial.println(notification->title);
//   Serial.println(notification->message);
//   Serial.println(notification->type);  
}

void create_notif_messagebox(String title, String message) {
//  static const char * btns[] ={"Close", ""};
  if (notif_messagebox_exists) {
    lv_obj_del(notif_messagebox);
  }
    
  notif_messagebox = lv_msgbox_create(lv_scr_act(), NULL);
  lv_msgbox_set_text(notif_messagebox, (title + "\n" + message).c_str());
//  lv_msgbox_add_btns(notif_messagebox, btns);
  lv_msgbox_start_auto_close(notif_messagebox, 10000);
  lv_obj_set_width(notif_messagebox, 200);
  lv_obj_set_event_cb(notif_messagebox, notif_messagebox_handler);
  lv_obj_align(notif_messagebox, NULL, LV_ALIGN_CENTER, 0, 0);

  notif_messagebox_exists = true;
}

static void notif_messagebox_handler(lv_obj_t * obj, lv_event_t event) {
  if (event == LV_EVENT_CLICKED) {
//    lv_msgbox_close(notif_messagebox);
  }
  if (event == LV_EVENT_DELETE) {
    notif_messagebox_exists = false;
  }
}
