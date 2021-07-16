
void switch_screen(uint8_t _screen);
void prepare_screen(uint8_t _screen);

void switch_screen(uint8_t _screen) {
//  setCpuFrequencyMhz(HIGH_CPU);
  prepare_screen(_screen);
  lv_scr_load_anim(prepared_screen, LV_SCR_LOAD_ANIM_NONE, TRANSITION_TIME, 0, true);
  current_screen = _screen;
//  setCpuFrequencyMhz(LOW_CPU);
}

void switch_screen_fade(uint8_t _screen) {
//  setCpuFrequencyMhz(HIGH_CPU);
  prepare_screen(_screen);
  if (use_transitions) {
    lv_scr_load_anim(prepared_screen, LV_SCR_LOAD_ANIM_FADE_ON, TRANSITION_TIME, 0, true);
  } else {
    lv_scr_load_anim(prepared_screen, LV_SCR_LOAD_ANIM_NONE, 0, 0, true);
  }
  current_screen = _screen;
//  setCpuFrequencyMhz(LOW_CPU);
}

void prepare_screen(uint8_t _screen) {
  lv_obj_t * scr = lv_obj_create(NULL, NULL);

  switch (_screen) {
    case BLANK_SCREEN: {
      lv_obj_set_style_local_bg_color(scr, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLACK);
      
      break;
    }

    case CLOCK_SCREEN: {

      switch (current_watch_face) {

        case MINIMALIST_DIGITAL_FACE: {
          lv_obj_set_style_local_bg_color(scr, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLACK);
          
          lv_obj_t * clock_label = lv_label_create(scr, NULL);
          lv_obj_add_style(clock_label, LV_LABEL_PART_MAIN, &label_style);
          lv_obj_add_style(clock_label, LV_LABEL_PART_MAIN, &giant_label_style);
          lv_obj_set_auto_realign(clock_label, true);
          lv_obj_align(clock_label, NULL, LV_ALIGN_CENTER, 0, 0);
          
          lv_obj_set_event_cb(scr, change_screens_menu_cb);
          
          update_screen_time(scr);
          
          break;
        }

        case ANALOG_FACE: {
          lv_obj_set_style_local_bg_color(scr, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLACK);

          static lv_color_t main_needle_colors[1] = {LV_COLOR_WHITE};
          
          lv_obj_t * main_gauge = lv_gauge_create(scr, NULL);
          lv_obj_add_style(main_gauge, LV_GAUGE_PART_MAIN, &analog_clock_face_gauge_style);
          lv_obj_add_style(main_gauge, LV_GAUGE_PART_MAIN, &analog_clock_face_gauge_minor_line_style);
          lv_obj_add_style(main_gauge, LV_GAUGE_PART_MAJOR, &analog_clock_face_gauge_major_line_style);
          lv_obj_set_size(main_gauge, 240, 240);
          lv_gauge_set_range(main_gauge, 0, 12);
          lv_gauge_set_scale(main_gauge, 360, 61, 13);
          lv_gauge_set_critical_value(main_gauge, 12);
          lv_gauge_set_angle_offset(main_gauge, 180);
          lv_gauge_set_needle_count(main_gauge, 1, main_needle_colors);
          lv_obj_align(main_gauge, NULL, LV_ALIGN_CENTER, 0, 0);

          analog_clock_main_gauge = main_gauge;

          static lv_color_t second_needle_colors[2] = {LV_COLOR_GREY, LV_COLOR_RED};

          lv_obj_t * second_gauge = lv_gauge_create(scr, NULL);
          lv_obj_add_style(second_gauge, LV_GAUGE_PART_MAIN, &analog_clock_face_gauge_null_style);
          lv_obj_add_style(second_gauge, LV_GAUGE_PART_MAJOR, &analog_clock_face_gauge_null_style);
          lv_obj_set_size(second_gauge, 240, 240);
          lv_gauge_set_range(second_gauge, 0, 60);
          lv_gauge_set_scale(second_gauge, 360, 0, 0);
          lv_gauge_set_angle_offset(second_gauge, 180);
          lv_gauge_set_needle_count(second_gauge, 2, second_needle_colors);
          lv_obj_align(second_gauge, NULL, LV_ALIGN_CENTER, 0, 0);

          analog_clock_second_gauge = second_gauge;

          update_screen_time(scr);
          
          lv_obj_set_event_cb(scr, change_screens_menu_cb);
          lv_obj_set_event_cb(second_gauge, change_screens_menu_cb);
          
          break;
        }

        case BG_1_FACE: {
          lv_obj_set_style_local_bg_color(scr, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLACK);

          lv_obj_t * bg_img = lv_img_create(scr, NULL);
          lv_img_set_src(bg_img, &background_1);
          lv_obj_align(bg_img, NULL, LV_ALIGN_CENTER, 0, 0);
          
          lv_obj_set_event_cb(scr, change_screens_menu_cb);

          lv_obj_t * clock_label = lv_label_create(scr, NULL);
          lv_obj_add_style(clock_label, LV_LABEL_PART_MAIN, &label_style);
//          lv_obj_add_style(clock_label, LV_LABEL_PART_MAIN, &giant_label_style);
          lv_obj_set_auto_realign(clock_label, true);
          lv_obj_align(clock_label, NULL, LV_ALIGN_IN_BOTTOM_LEFT, 20, -20);
          
          lv_obj_set_event_cb(scr, change_screens_menu_cb);
          
          update_screen_time(scr);
          
          break;
        }
        
      }
      
      break;
    }

    case MENU_SCREEN: {
      lv_obj_set_style_local_bg_color(scr, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLACK);

      uint8_t button_size = (240.0 - (GLOBAL_PADDING * 6.0)) / 2.0;

      lv_obj_t * button_1;
      lv_obj_t * button_2;
      lv_obj_t * button_3;
      lv_obj_t * button_4;

      lv_obj_t * button_label_1;
      lv_obj_t * button_label_2;
      lv_obj_t * button_label_3;
      lv_obj_t * button_label_4;

      button_1 = lv_btn_create(scr, NULL);
      lv_obj_add_style(button_1, LV_BTN_PART_MAIN, &button_style);
      lv_obj_set_size(button_1, button_size, button_size);
      lv_obj_align(button_1, NULL, LV_ALIGN_IN_TOP_LEFT, GLOBAL_PADDING * 2, GLOBAL_PADDING * 2);

      button_label_1 = lv_label_create(button_1, NULL);
      lv_obj_add_style(button_label_1, LV_LABEL_PART_MAIN, &label_style);
      lv_obj_add_style(button_label_1, LV_LABEL_PART_MAIN, &small_label_style);
      
      button_2 = lv_btn_create(scr, button_1);
      button_3 = lv_btn_create(scr, button_1);
      button_4 = lv_btn_create(scr, button_1);

      lv_obj_align(button_2, NULL, LV_ALIGN_IN_TOP_RIGHT, -(GLOBAL_PADDING * 2), (GLOBAL_PADDING * 2));
      lv_obj_align(button_3, NULL, LV_ALIGN_IN_BOTTOM_LEFT, (GLOBAL_PADDING * 2), -(GLOBAL_PADDING * 2));
      lv_obj_align(button_4, NULL, LV_ALIGN_IN_BOTTOM_RIGHT, -(GLOBAL_PADDING * 2), -(GLOBAL_PADDING * 2));

      button_label_2 = lv_label_create(button_2, button_label_1);
      button_label_3 = lv_label_create(button_3, button_label_1);
      button_label_4 = lv_label_create(button_4, button_label_1);

      lv_label_set_text(button_label_1, "back");
      lv_label_set_text(button_label_2, "apps");
      lv_label_set_text(button_label_3, "un-\nused");
      lv_label_set_text(button_label_4, "settings");
      
      lv_obj_set_event_cb(button_1, change_screens_clock_cb);
      lv_obj_set_event_cb(button_2, change_screens_apps_cb);
      lv_obj_set_event_cb(button_4, change_screens_settings_cb);
      
      break;
    }

    case SETTINGS_SCREEN: {
      lv_obj_set_style_local_bg_color(scr, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLACK);

      lv_obj_t * screen_label = lv_label_create(scr, NULL);
      lv_obj_add_style(screen_label, LV_LABEL_PART_MAIN, &label_style);
      lv_label_set_text(screen_label, "Settings");
      lv_obj_align(screen_label, NULL, LV_ALIGN_IN_TOP_LEFT, GLOBAL_PADDING, GLOBAL_PADDING);

      lv_obj_t * back_button = lv_btn_create(scr, NULL);
      setup_standard_button(back_button, "back");
      lv_obj_set_event_cb(back_button, change_screens_menu_cb);
      lv_obj_align(back_button, NULL, LV_ALIGN_IN_TOP_RIGHT, -GLOBAL_PADDING, GLOBAL_PADDING);

      lv_obj_t * page = lv_page_create(scr, NULL);
      lv_obj_set_style_local_bg_color(page, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLACK);
      lv_obj_add_style(page, LV_PAGE_PART_BG, &page_style);
      lv_obj_add_style(page, LV_PAGE_PART_SCROLLABLE, &inner_page_style);
      lv_obj_set_size(page, 240, 240 - max(lv_obj_get_height(back_button), lv_obj_get_height(screen_label)) - (GLOBAL_PADDING * 2));
      lv_obj_align(page, NULL, LV_ALIGN_IN_TOP_LEFT, 0, max(lv_obj_get_height(back_button), lv_obj_get_height(screen_label)) + (GLOBAL_PADDING * 2));
      lv_page_set_scrl_layout(page, LV_LAYOUT_COLUMN_LEFT);

      lv_obj_t * bl_slider_label = lv_label_create(page, NULL);
      lv_obj_add_style(bl_slider_label, LV_LABEL_PART_MAIN, &label_style);
      lv_obj_add_style(bl_slider_label, LV_LABEL_PART_MAIN, &small_label_style);
      lv_label_set_text(bl_slider_label, "Backlight");

      lv_obj_t * bl_slider = lv_slider_create(page, NULL);
      lv_obj_set_width(bl_slider, 190);
      lv_slider_set_range(bl_slider, 5, 255);
      lv_slider_set_value(bl_slider, last_bl_value, false);
      lv_obj_set_event_cb(bl_slider, set_bl_from_slider);

      lv_obj_t * watch_face_button = lv_btn_create(page, NULL);
      setup_standard_button(watch_face_button, "watch face");
      lv_obj_set_event_cb(watch_face_button, change_screens_watch_face_cb);

      lv_obj_t * wifi_button = lv_btn_create(page, NULL);
      setup_standard_button(wifi_button, "wifi");
      lv_obj_set_event_cb(wifi_button, change_screens_wifi_cb);

      lv_obj_t * battery_button = lv_btn_create(page, NULL);
      setup_standard_button(battery_button, "battery");
      lv_obj_set_event_cb(battery_button, change_screens_battery_cb);
      
      break;
    }

    case WATCH_FACE_SCREEN: {
      lv_obj_set_style_local_bg_color(scr, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLACK);

      lv_obj_t * screen_label = lv_label_create(scr, NULL);
      lv_obj_add_style(screen_label, LV_LABEL_PART_MAIN, &label_style);
      lv_label_set_text(screen_label, "Watch\nFace");
      lv_obj_align(screen_label, NULL, LV_ALIGN_IN_TOP_LEFT, GLOBAL_PADDING, GLOBAL_PADDING);

      lv_obj_t * back_button = lv_btn_create(scr, NULL);
      setup_standard_button(back_button, "back");
      lv_obj_set_event_cb(back_button, change_screens_settings_cb);
      lv_obj_align(back_button, NULL, LV_ALIGN_IN_TOP_RIGHT, -GLOBAL_PADDING, GLOBAL_PADDING);

      lv_obj_t * drop_down = lv_dropdown_create(scr, NULL);
      lv_dropdown_set_options(drop_down, "minimalist digital\nanalog\nspace digital");
      lv_obj_set_width(drop_down, 240 - GLOBAL_PADDING*2);
      lv_obj_align(drop_down, NULL, LV_ALIGN_IN_TOP_LEFT, GLOBAL_PADDING, max(lv_obj_get_height(back_button), lv_obj_get_height(screen_label)) + (GLOBAL_PADDING * 2));
      lv_obj_set_event_cb(drop_down, change_watch_face_cb);

      switch (current_watch_face) {

        case MINIMALIST_DIGITAL_FACE: {
          lv_dropdown_set_selected(drop_down, 0);
          
          break;
        }

        case ANALOG_FACE: {
          lv_dropdown_set_selected(drop_down, 1);

          break;
        }

        case BG_1_FACE: {
          lv_dropdown_set_selected(drop_down, 2);
          
          break;
        }
      }
      
      break;
    }

    case BATTERY_SCREEN: {
      lv_obj_set_style_local_bg_color(scr, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLACK);

      lv_obj_t * screen_label = lv_label_create(scr, NULL);
      lv_obj_add_style(screen_label, LV_LABEL_PART_MAIN, &label_style);
      lv_label_set_text(screen_label, "Battery");
      lv_obj_align(screen_label, NULL, LV_ALIGN_IN_TOP_LEFT, GLOBAL_PADDING, GLOBAL_PADDING);

      lv_obj_t * back_button = lv_btn_create(scr, NULL);
      setup_standard_button(back_button, "back");
      lv_obj_set_event_cb(back_button, change_screens_settings_cb);
      lv_obj_align(back_button, NULL, LV_ALIGN_IN_TOP_RIGHT, -GLOBAL_PADDING, GLOBAL_PADDING);

      lv_obj_t * page = lv_page_create(scr, NULL);
      lv_obj_set_style_local_bg_color(page, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLACK);
      lv_obj_add_style(page, LV_PAGE_PART_BG, &page_style);
      lv_obj_add_style(page, LV_PAGE_PART_SCROLLABLE, &inner_page_style);
      lv_obj_set_size(page, 240, 240 - max(lv_obj_get_height(back_button), lv_obj_get_height(screen_label)) - (GLOBAL_PADDING * 2));
      lv_obj_align(page, NULL, LV_ALIGN_IN_TOP_LEFT, 0, max(lv_obj_get_height(back_button), lv_obj_get_height(screen_label)) + (GLOBAL_PADDING * 2));
      lv_page_set_scrl_layout(page, LV_LAYOUT_COLUMN_LEFT);

      lv_obj_t * battery_label = lv_label_create(page, NULL);
      lv_obj_add_style(battery_label, LV_LABEL_PART_MAIN, &label_style);
      lv_obj_add_style(battery_label, LV_LABEL_PART_MAIN, &small_label_style);
//      lv_obj_align(battery_label, NULL, LV_ALIGN_IN_TOP_LEFT, GLOBAL_PADDING, max(lv_obj_get_height(back_button), lv_obj_get_height(screen_label)) + (GLOBAL_PADDING * 2));
      lv_label_set_recolor(battery_label, true);
      lv_label_set_long_mode(battery_label, LV_LABEL_LONG_BREAK);
      lv_obj_set_width(battery_label, 240 - (GLOBAL_PADDING * 2));
      
      battery_label_reference = battery_label;
      update_battery_label();

      lv_obj_t * chart = lv_chart_create(page, NULL);
      lv_obj_set_size(chart, 220, 150);
      lv_chart_set_type(chart, LV_CHART_TYPE_LINE);
      lv_chart_set_range(chart, 0, 100);
      lv_page_glue_obj(chart, true);

      lv_chart_series_t * ser1 = lv_chart_add_series(chart, LV_COLOR_GREEN);
      lv_chart_set_ext_array(chart, ser1, battery_percentage_history, BATTERY_PERCENTAGE_HISTORY_READINGS);
      lv_chart_refresh(chart);
      
      break;
    }

    case WIFI_SCREEN: {
      lv_obj_set_style_local_bg_color(scr, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLACK);
      
      lv_obj_t * screen_label = lv_label_create(scr, NULL);
      lv_obj_add_style(screen_label, LV_LABEL_PART_MAIN, &label_style);
      lv_label_set_text(screen_label, "WiFi");
      lv_obj_align(screen_label, NULL, LV_ALIGN_IN_TOP_LEFT, GLOBAL_PADDING, GLOBAL_PADDING);

      lv_obj_t * back_button = lv_btn_create(scr, NULL);
      setup_standard_button(back_button, "back");
      lv_obj_set_event_cb(back_button, change_screens_settings_cb);
      lv_obj_align(back_button, NULL, LV_ALIGN_IN_TOP_RIGHT, -GLOBAL_PADDING, GLOBAL_PADDING);

      lv_obj_t * toggle = lv_switch_create(scr, NULL);
      lv_obj_add_style(toggle, LV_LABEL_PART_MAIN, &switch_style);
      lv_obj_align(toggle, NULL, LV_ALIGN_IN_TOP_LEFT, GLOBAL_PADDING, max(lv_obj_get_height(back_button), lv_obj_get_height(screen_label)) + (GLOBAL_PADDING * 2));
      lv_obj_set_event_cb(toggle, change_wifi_active_cb);

      wifi_toggle = toggle;

      if (wifi_active) { lv_switch_on(toggle, LV_ANIM_OFF); } else { lv_switch_off(toggle, LV_ANIM_OFF); }
      
      lv_obj_t * toggle_label = lv_label_create(scr, NULL);
      lv_obj_add_style(toggle_label, LV_LABEL_PART_MAIN, &label_style);
      lv_obj_add_style(toggle_label, LV_LABEL_PART_MAIN, &small_label_style);
      lv_label_set_text(toggle_label, "Enable");
      lv_obj_align(toggle_label, toggle, LV_ALIGN_OUT_RIGHT_MID, GLOBAL_PADDING, 0);

      wifi_status_label = lv_label_create(scr, NULL);
      lv_obj_add_style(wifi_status_label, LV_LABEL_PART_MAIN, &label_style);
      lv_obj_add_style(wifi_status_label, LV_LABEL_PART_MAIN, &small_label_style);
      lv_label_set_text(wifi_status_label, "Getting WiFi status...");
      lv_obj_align(wifi_status_label, NULL, LV_ALIGN_IN_TOP_LEFT, GLOBAL_PADDING, lv_obj_get_y(toggle) + lv_obj_get_height(toggle) + GLOBAL_PADDING);
      
      break;
    }

    case APPS_SCREEN: {
      lv_obj_set_style_local_bg_color(scr, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLACK);
      
      lv_obj_t * screen_label = lv_label_create(scr, NULL);
      lv_obj_add_style(screen_label, LV_LABEL_PART_MAIN, &label_style);
      lv_label_set_text(screen_label, "Apps");
      lv_obj_align(screen_label, NULL, LV_ALIGN_IN_TOP_LEFT, GLOBAL_PADDING, GLOBAL_PADDING);

      lv_obj_t * back_button = lv_btn_create(scr, NULL);
      setup_standard_button(back_button, "back");
      lv_obj_set_event_cb(back_button, change_screens_menu_cb);
      lv_obj_align(back_button, NULL, LV_ALIGN_IN_TOP_RIGHT, -GLOBAL_PADDING, GLOBAL_PADDING);
      
      lv_obj_t * page = lv_page_create(scr, NULL);
      lv_obj_set_style_local_bg_color(page, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLACK);
      lv_obj_add_style(page, LV_PAGE_PART_BG, &page_style);
      lv_obj_add_style(page, LV_PAGE_PART_SCROLLABLE, &inner_page_style);
      lv_obj_set_size(page, 240, 240 - max(lv_obj_get_height(back_button), lv_obj_get_height(screen_label)) - (GLOBAL_PADDING * 2));
      lv_obj_align(page, NULL, LV_ALIGN_IN_TOP_LEFT, 0, max(lv_obj_get_height(back_button), lv_obj_get_height(screen_label)) + (GLOBAL_PADDING * 2));
      lv_page_set_scrl_layout(page, LV_LAYOUT_COLUMN_LEFT);

      lv_obj_t * printer_button = lv_btn_create(page, NULL);
      setup_standard_button(printer_button, "printer status");
      lv_obj_set_event_cb(printer_button, change_screens_printer_cb);
      
      break;
    }

    case APP_PRINTER_SCREEN: {
      lv_obj_set_style_local_bg_color(scr, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLACK);
      
      lv_obj_t * screen_label = lv_label_create(scr, NULL);
      lv_obj_add_style(screen_label, LV_LABEL_PART_MAIN, &label_style);
      lv_label_set_text(screen_label, "Printer\nStatus");
      lv_obj_align(screen_label, NULL, LV_ALIGN_IN_TOP_LEFT, GLOBAL_PADDING, GLOBAL_PADDING);

      lv_obj_t * back_button = lv_btn_create(scr, NULL);
      setup_standard_button(back_button, "back");
      lv_obj_set_event_cb(back_button, change_screens_apps_cb);
      lv_obj_align(back_button, NULL, LV_ALIGN_IN_TOP_RIGHT, -GLOBAL_PADDING, GLOBAL_PADDING);

      lv_obj_t * page = lv_page_create(scr, NULL);
      lv_obj_set_style_local_bg_color(page, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLACK);
      lv_obj_add_style(page, LV_PAGE_PART_BG, &page_style);
      lv_obj_add_style(page, LV_PAGE_PART_SCROLLABLE, &inner_page_style);
      lv_obj_set_size(page, 240, 240 - max(lv_obj_get_height(back_button), lv_obj_get_height(screen_label)) - (GLOBAL_PADDING * 2));
      lv_obj_align(page, NULL, LV_ALIGN_IN_TOP_LEFT, 0, max(lv_obj_get_height(back_button), lv_obj_get_height(screen_label)) + (GLOBAL_PADDING * 2));
      lv_page_set_scrl_layout(page, LV_LAYOUT_COLUMN_LEFT);

      lv_obj_t * refresh_button = lv_btn_create(page, NULL);
      setup_standard_button(refresh_button, "refresh status");
      lv_obj_set_event_cb(refresh_button, refresh_printer_status);
      lv_obj_align(refresh_button, NULL, LV_ALIGN_IN_TOP_RIGHT, -GLOBAL_PADDING, GLOBAL_PADDING);

      lv_obj_t * status_label = lv_label_create(page, NULL);
      lv_obj_add_style(status_label, LV_LABEL_PART_MAIN, &label_style);
      lv_obj_add_style(status_label, LV_LABEL_PART_MAIN, &small_label_style);
      lv_label_set_recolor(status_label, true);
      lv_label_set_long_mode(status_label, LV_LABEL_LONG_BREAK);
      lv_obj_set_width(status_label, 240 - (GLOBAL_PADDING * 2));

      printer_status_label = status_label;

      lv_label_set_text(status_label, "Printer status unknown");
      
      break;
    }
    
  }

  prepared_screen = scr;
}

static void change_watch_face_cb(lv_obj_t * obj, lv_event_t event) {
  if (event == LV_EVENT_VALUE_CHANGED) {
    switch (lv_dropdown_get_selected(obj)) {

      case 0: {
        current_watch_face = MINIMALIST_DIGITAL_FACE;
        break;
      }

      case 1: {
        current_watch_face = ANALOG_FACE;
        break;
      }

      case 2: {
        current_watch_face = BG_1_FACE;
        break;
      }
      
    }
  }
}

static void change_wifi_active_cb(lv_obj_t * obj, lv_event_t event) {
  if (event == LV_EVENT_VALUE_CHANGED) {
    wifi_active = lv_switch_get_state(obj);
  }
}

static void change_screens_clock_cb(lv_obj_t * obj, lv_event_t event) {
  if (event == LV_EVENT_CLICKED) {
    switch_screen_fade(CLOCK_SCREEN);
  }
}

static void change_screens_menu_cb(lv_obj_t * obj, lv_event_t event) {
  if (event == LV_EVENT_CLICKED) {
    switch_screen_fade(MENU_SCREEN);
  }
}

static void change_screens_settings_cb(lv_obj_t * obj, lv_event_t event) {
  if (event == LV_EVENT_CLICKED) {
    switch_screen_fade(SETTINGS_SCREEN);
  }
}

static void change_screens_watch_face_cb(lv_obj_t * obj, lv_event_t event) {
  if (event == LV_EVENT_CLICKED) {
    switch_screen_fade(WATCH_FACE_SCREEN);
  }
}

static void change_screens_battery_cb(lv_obj_t * obj, lv_event_t event) {
  if (event == LV_EVENT_CLICKED) {
    switch_screen_fade(BATTERY_SCREEN);
  }
}

static void change_screens_wifi_cb(lv_obj_t * obj, lv_event_t event) {
  if (event == LV_EVENT_CLICKED) {
    switch_screen_fade(WIFI_SCREEN);
  }
}

static void change_screens_apps_cb(lv_obj_t * obj, lv_event_t event) {
  if (event == LV_EVENT_CLICKED) {
    switch_screen_fade(APPS_SCREEN);
  }
}

static void change_screens_printer_cb(lv_obj_t * obj, lv_event_t event) {
  if (event == LV_EVENT_CLICKED) {
    switch_screen_fade(APP_PRINTER_SCREEN);
  }
}

static void set_bl_from_slider(lv_obj_t * obj, lv_event_t event) {
  if (event == LV_EVENT_VALUE_CHANGED) {
    set_bl(lv_slider_get_value(obj));
  }
}
