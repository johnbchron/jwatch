/*
 * 
 * In order to add a new screen:
 *  1.  add it in the enum in the master file
 *  2.  add a case for it in the prepare_screen() function in the screens file
 *  3.  add a change_screens_[SCREEN_NAME]_cb lvgl callback to switch to it in the screens file 
 *  4.  assign that callback to a button on some other screen
 *  5.  add a case in button_short_press() in the power file
 *  6.  add a case in the update_screens_task() function in the tasks file if needed
 *  
 * that's it!
 *  
 * In order to add a new watch face:
 *  1.  add it in the enum in the master file
 *  2.  add a case for it in the CLOCK_SCREEN case of the prepare_screen() function in the screens file
 *  3.  add a cosmetic name in the text options of the dropdown in the watch face case in the screens file
 *  3.  add a case for it to pre-select current watch face for the dropdown in the watch face case in the screens file
 *  4.  add a case to change to the new face in the change_watch_face_cb() callback
 *  
 * that's it!
 *  
 */
