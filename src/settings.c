#include <pebble.h>
#include "pebblodoro.h"
#include "settings.h"

/** Static Variables **/
static Window *s_settings_window;
static SimpleMenuLayer *s_settings_menu;
static SimpleMenuSection s_settings_menu_sections;

/** Static Functions **/
static void settings_menu_callback(int, void *);
static void settings_window_load(Window *);

/** Code **/

void load_settings() {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "load_settings");
  s_settings_window = window_create();
  window_set_window_handlers(s_settings_window, (WindowHandlers) {
    .load = settings_window_load,
    .unload = window_unload,
  });
  window_stack_push(s_settings_window, true);
}

static void settings_window_load(Window *window) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "settings_window_load");
  // Set the current window to settings
  current_window = WINDOW_SETTINGS;
  // Create the window's child layers
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  const int32_t num_sections = 2;
  static SimpleMenuItem s_settings_menu_items[8];
  s_settings_menu_items[0] = (SimpleMenuItem) {
    .title = "Set Focus Time",
    .subtitle = NULL,
    .icon = NULL,
    .callback = settings_menu_callback
  };
  s_settings_menu_items[1] = (SimpleMenuItem) {
    .title = "Set Break Time",
    .subtitle = NULL,
    .icon = NULL,
    .callback = settings_menu_callback
  };
  s_settings_menu_sections = (SimpleMenuSection){
    .title = "Settings",
    .items = s_settings_menu_items,
    .num_items = num_sections
  };
  s_settings_menu = simple_menu_layer_create(
    bounds,
    window,
    &s_settings_menu_sections,
    num_sections,
    settings_menu_callback);
  layer_add_child(window_layer, simple_menu_layer_get_layer(s_settings_menu));
}

static void settings_menu_callback(int index, void *context) {
  switch(index) {
    case 0:
    break
  }
}

static void set_timer(int set_focus) {

}
