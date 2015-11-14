#include <pebble.h>
#include "pebblodoro.h"
#include "settings.h"

/** Static Variables **/
static Window *s_settings_window;
static SimpleMenuLayer *s_settings_menu;
static SimpleMenuSection s_settings_menu_sections;

/** Static Functions **/
static void settings_menu_callback(int, void *);
