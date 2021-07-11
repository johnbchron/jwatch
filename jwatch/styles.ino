
void setup_styles() {
  lv_style_init(&label_style);
  lv_style_set_text_color(&label_style, LV_STATE_DEFAULT, LV_COLOR_WHITE);
  lv_style_set_text_font(&label_style, LV_STATE_DEFAULT, &lv_font_montserrat_24);
  
  lv_style_init(&big_label_style);
  lv_style_set_text_font(&big_label_style, LV_STATE_DEFAULT, &lv_font_montserrat_32);
  
  lv_style_init(&small_label_style);
  lv_style_set_text_font(&small_label_style, LV_STATE_DEFAULT, &lv_font_montserrat_16);

  lv_style_init(&giant_label_style);
  lv_style_set_text_font(&giant_label_style, LV_STATE_DEFAULT, &lv_font_montserrat_48);
  
  lv_style_init(&button_style);
  lv_style_set_radius(&button_style, LV_STATE_DEFAULT, 10);
  lv_style_set_bg_color(&button_style, LV_STATE_DEFAULT, LV_COLOR_BLACK);
  lv_style_set_pad_top(&button_style, LV_STATE_DEFAULT, GLOBAL_PADDING*1.5);
  lv_style_set_pad_bottom(&button_style, LV_STATE_DEFAULT, GLOBAL_PADDING*1.5);
  lv_style_set_pad_left(&button_style, LV_STATE_DEFAULT, GLOBAL_PADDING*4);
  lv_style_set_pad_right(&button_style, LV_STATE_DEFAULT, GLOBAL_PADDING*4);
  lv_style_set_margin_top(&button_style, LV_STATE_DEFAULT, 0);
  lv_style_set_margin_bottom(&button_style, LV_STATE_DEFAULT, 0);
  lv_style_set_margin_left(&button_style, LV_STATE_DEFAULT, 0);
  lv_style_set_margin_right(&button_style, LV_STATE_DEFAULT, 0);
  if (!use_transitions) {
    lv_style_set_transition_time(&button_style, LV_STATE_DEFAULT, 0);
    lv_style_set_transition_time(&button_style, LV_STATE_PRESSED, 0);
  }

  lv_style_init(&switch_style);
  lv_style_set_bg_color(&switch_style, LV_STATE_DEFAULT, LV_COLOR_BLACK);
  lv_style_set_border_opa(&switch_style, LV_STATE_DEFAULT, LV_OPA_COVER);
  lv_style_set_border_color(&switch_style, LV_STATE_DEFAULT, LV_COLOR_WHITE);
  lv_style_set_border_color(&switch_style, LV_STATE_CHECKED, LV_COLOR_WHITE);
  lv_style_set_border_width(&switch_style, LV_STATE_DEFAULT, 2);
  if (!use_transitions) {
    lv_style_set_transition_time(&switch_style, LV_STATE_DEFAULT, 0);
    lv_style_set_transition_time(&switch_style, LV_STATE_PRESSED, 0);
  }

  lv_style_init(&page_style);
  lv_style_set_radius(&page_style, LV_STATE_DEFAULT, 0);
  lv_style_set_border_opa(&page_style, LV_STATE_DEFAULT, LV_OPA_TRANSP);
  lv_style_set_pad_top(&page_style, LV_STATE_DEFAULT, GLOBAL_PADDING);
  lv_style_set_pad_bottom(&page_style, LV_STATE_DEFAULT, GLOBAL_PADDING);
  lv_style_set_pad_left(&page_style, LV_STATE_DEFAULT, GLOBAL_PADDING);
  lv_style_set_pad_right(&page_style, LV_STATE_DEFAULT, GLOBAL_PADDING);

  lv_style_init(&inner_page_style);
  lv_style_set_pad_inner(&inner_page_style, LV_STATE_DEFAULT, GLOBAL_PADDING);

  lv_style_init(&analog_clock_face_gauge_style);
  lv_style_set_bg_color(&analog_clock_face_gauge_style, LV_STATE_DEFAULT, LV_COLOR_BLACK);
  lv_style_set_border_opa(&analog_clock_face_gauge_style, LV_STATE_DEFAULT, LV_OPA_TRANSP);
  lv_style_set_pad_inner(&analog_clock_face_gauge_style, LV_STATE_DEFAULT, GLOBAL_PADDING * 3);
  lv_style_set_text_color(&analog_clock_face_gauge_style, LV_STATE_DEFAULT, LV_COLOR_WHITE);

  lv_style_init(&analog_clock_face_gauge_major_line_style);
  lv_style_set_line_color(&analog_clock_face_gauge_major_line_style, LV_STATE_DEFAULT, LV_COLOR_WHITE);
  lv_style_set_line_width(&analog_clock_face_gauge_major_line_style, LV_STATE_DEFAULT, 3);
  
  lv_style_init(&analog_clock_face_gauge_minor_line_style);
  lv_style_set_line_color(&analog_clock_face_gauge_minor_line_style, LV_STATE_DEFAULT, LV_COLOR_GREY);
  lv_style_set_line_width(&analog_clock_face_gauge_minor_line_style, LV_STATE_DEFAULT, 1);

  lv_style_init(&analog_clock_face_gauge_null_style);
  lv_style_set_text_opa(&analog_clock_face_gauge_null_style, LV_STATE_DEFAULT, LV_OPA_TRANSP);
  lv_style_set_line_opa(&analog_clock_face_gauge_null_style, LV_STATE_DEFAULT, LV_OPA_TRANSP);
  lv_style_set_bg_opa(&analog_clock_face_gauge_null_style, LV_STATE_DEFAULT, LV_OPA_TRANSP);
  lv_style_set_border_color(&analog_clock_face_gauge_null_style, LV_STATE_DEFAULT, LV_COLOR_WHITE);
  lv_style_set_border_color(&analog_clock_face_gauge_null_style, LV_STATE_PRESSED, LV_COLOR_WHITE);
  lv_style_set_border_color(&analog_clock_face_gauge_null_style, LV_STATE_FOCUSED, LV_COLOR_WHITE);
}

void setup_standard_button(lv_obj_t * btn, String label_text) {
  lv_obj_add_style(btn, LV_BTN_PART_MAIN, &button_style);

  lv_obj_t * btn_label = lv_label_create(btn, NULL);
  lv_obj_add_style(btn_label, LV_LABEL_PART_MAIN, &label_style);
  lv_obj_add_style(btn_label, LV_LABEL_PART_MAIN, &small_label_style);
  lv_label_set_text(btn_label, label_text.c_str());

  lv_btn_set_fit2(btn, LV_FIT_TIGHT, LV_FIT_TIGHT);
}
