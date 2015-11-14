#include <pebble.h>
#include "main_menu.h"
#include "pebblodoro.h"

/** Static Variables **/
static SimpleMenuLayer *s_main_menu;
static SimpleMenuSection s_main_menu_sections;

/** Static Functions **/
static void main_menu_callback(int , void *);


void main_window_load(Window *window) {
  // Make sure we know what the current screen is
  current_window = WINDOW_MAIN_MENU;
  // Create Window's child Layers here
  Layer *window_layer = window_get_root_layer(s_main_window);
  GRect bounds = layer_get_bounds(window_layer);
  const int32_t num_sections = 3;
  static SimpleMenuItem s_main_menu_items[8];
  s_main_menu_items[0] = (SimpleMenuItem) {
    .title = "Start Focus",
    .subtitle = NULL,
    .icon = NULL,
    .callback = main_menu_callback
  };
  s_main_menu_items[1] = (SimpleMenuItem) {
    .title = "Start Break",
    .subtitle = NULL,
    .icon = NULL,
    .callback = main_menu_callback
  };
  s_main_menu_items[2] = (SimpleMenuItem) {
    .title = "Settings",
    .subtitle = NULL,
    .icon = NULL,
    .callback = main_menu_callback
  };
  s_main_menu_sections = (SimpleMenuSection){
    .title = "Pebblodoro",
    .items = s_main_menu_items,
    .num_items = num_sections
  };
  s_main_menu = simple_menu_layer_create(
    bounds,
    s_main_window,
    &s_main_menu_sections,
    num_sections,
    main_menu_callback);
  layer_add_child(window_layer, simple_menu_layer_get_layer(s_main_menu));
}

static void main_menu_callback(int index, void *context) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "MAINMENU:%d", index);
  switch (index) {
    case 0:
      start_focus();
      break;
    case 1:
      start_break();
      break;
    break;
    default:
    break;
  }
  return;
}
