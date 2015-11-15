#include <pebble.h>
#include "pebblodoro.h"
#include "main_menu.h"

/** Static Variables **/
static Window *s_countdown_window;
static TextLayer *s_label_layer;
static BitmapLayer *s_icon_layer;
static GBitmap *s_icon_bitmap, *s_tick_bitmap, *s_cross_bitmap, *s_ellipsis_bitmap;
static int focus_time = 25 ;//*60;
static int break_time = 5 ;//*60;
static int focus_extra_time = 5 *60;
static int break_extra_time = 1 *60;
static int current = 0;
static ActionBarLayer *action_bar;


/** Static Functions **/
static void countdown_window_load(Window *);
static void select_click_handler(ClickRecognizerRef, void *);
static void draw_pommodoro(void);
static void countdown(int, int);
static void update_countdown_string(int);
static void break_or_extend(void);
static void break_or_extend_click_config_provider(void *);
static void extend_focus(void);
static void extend_break(void);

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {

}

static void click_config_provider(void *context) {
  // Register the ClickHandlers
  //window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  //window_single_click_subscribe(BUTTON_ID_BACK, select_click_handler);
  //window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}




static void init(void) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "init");
  s_main_window = window_create();
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = window_unload,
  });
  window_stack_push(s_main_window, true);
  if(persist_exists(FOCUS_TIME_KEY)) {
    focus_time = persist_read_int(FOCUS_TIME_KEY);
  }
  if(persist_exists(BREAK_TIME_KEY)) {
    break_time = persist_read_int(BREAK_TIME_KEY);
  }
  //window_set_click_config_provider(s_main_window, click_config_provider);
}

static void deinit(void) {
  window_destroy(s_main_window);
}

static void countdown_window_load(Window *window) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "countdown_window_load");
  current_window = WINDOW_COUNTDOWN;
  update_countdown_string(current);
}

void window_unload(Window *window) {
  // Destroy Window's child Layers here
  Layer *window_layer = window_get_root_layer(window);
  layer_remove_child_layers(window_layer);
}

static void draw_pommodoro() {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Drawing tomaito tomahto");
  Layer *window_layer = window_get_root_layer(s_countdown_window);
  GRect window_bounds = layer_get_bounds(window_layer);
  // Clear the window for the icon
  layer_remove_child_layers(window_layer);
  // Load the icon to its layer and add it to the window
  s_icon_bitmap = gbitmap_create_with_resource(RESOURCE_ID_POMMODORO);
  GRect bitmap_bounds = gbitmap_get_bounds(s_icon_bitmap);
  s_icon_layer = bitmap_layer_create(GRect(
    (window_bounds.size.w / 2) - (bitmap_bounds.size.w / 2),// - (ACTION_BAR_WIDTH / 2),
    10,
    bitmap_bounds.size.w,
    bitmap_bounds.size.h));
  bitmap_layer_set_bitmap(s_icon_layer, s_icon_bitmap);
  bitmap_layer_set_compositing_mode(s_icon_layer, GCompOpSet);
  layer_add_child(window_layer, bitmap_layer_get_layer(s_icon_layer));
}

static void update_countdown_string(int time) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "update_countdown_string");
  APP_LOG(APP_LOG_LEVEL_DEBUG, "current_window = %d", current_window);
  static char s_focus_buffer[32];
  APP_LOG(APP_LOG_LEVEL_DEBUG, "s_countdown_window.isloaded = %d", window_is_loaded(s_countdown_window));
  Layer *window_layer = window_get_root_layer(s_countdown_window);
  GRect window_bounds = layer_get_bounds(window_layer);
  GRect bitmap_bounds = gbitmap_get_bounds(s_icon_bitmap);
  // Add the countdown timer
  s_label_layer = text_layer_create(GRect(0, 5 + bitmap_bounds.size.h + 5, window_bounds.size.w, window_bounds.size.h));
  text_layer_set_text_alignment(s_label_layer, GTextAlignmentCenter);

  int seconds = time % 60;
  int minutes = (time % 3600) / 60;
  int hours = time / 3600;
  if(current_countdown_type == COUNTDOWN_TYPE_FOCUS) {
    snprintf(s_focus_buffer, sizeof(s_focus_buffer), "Focus: %dh %dm %ds", hours, minutes, seconds);
  }
  if(current_countdown_type == COUNTDOWN_TYPE_BREAK) {
    snprintf(s_focus_buffer, sizeof(s_focus_buffer), "Break: %dh %dm %ds", hours, minutes, seconds);
  }
  text_layer_set_text(s_label_layer, s_focus_buffer);
  layer_add_child(window_layer, text_layer_get_layer(s_label_layer));
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  current--;
  if(current == 0) {
    tick_timer_service_unsubscribe();
    break_or_extend();
  } else {
    update_countdown_string(current);
  }
}
static void countdown(int countdown_type, int is_extra) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "coundown type %d is_extra %d", countdown_type, is_extra);
  APP_LOG(APP_LOG_LEVEL_DEBUG, "current_window = %d", current_window);
  //focus_time = time;
  current_countdown_type = countdown_type;
  // Create countdown window
  if(current_window != WINDOW_COUNTDOWN){
    s_countdown_window = window_create();
    window_set_window_handlers(s_main_window, (WindowHandlers) {
      .load = countdown_window_load,
      .unload = window_unload,
    });
    current_window = WINDOW_COUNTDOWN;
  }
  window_stack_push(s_countdown_window, true);

  if(countdown_type == COUNTDOWN_TYPE_FOCUS)
  {
    current = (is_extra == COUNTDOWN_EXTRA_TIME ? focus_extra_time : focus_time);
  }
  else
  {
    current = (is_extra == COUNTDOWN_EXTRA_TIME ? break_extra_time : break_time);
  }

  draw_pommodoro();
  tick_timer_service_subscribe(SECOND_UNIT, tick_handler);
}
void start_focus() {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "start_focus");
  countdown(COUNTDOWN_TYPE_FOCUS, COUNTDOWN_NEW);
}
void start_break() {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "start_break");
  countdown(COUNTDOWN_TYPE_BREAK, COUNTDOWN_NEW);
}
static void extend_focus() {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "extend_focus");
  countdown(COUNTDOWN_TYPE_FOCUS, COUNTDOWN_EXTRA_TIME);
}
static void extend_break() {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "extend_break");
  countdown(COUNTDOWN_TYPE_BREAK, COUNTDOWN_EXTRA_TIME);
}
static void break_or_extend() {
  static char s_text_buffer[64];
  APP_LOG(APP_LOG_LEVEL_DEBUG, "BREAK OR EXTEND");

  Layer *window_layer = window_get_root_layer(s_countdown_window);
  GRect window_bounds = layer_get_bounds(window_layer);
  // Clear the window for the icon
  layer_remove_child_layers(window_layer);

  // Load the icon to its layer and add it to the window
  s_icon_bitmap = gbitmap_create_with_resource(RESOURCE_ID_CONFIRM);
  GRect bitmap_bounds = gbitmap_get_bounds(s_icon_bitmap);
  s_icon_layer = bitmap_layer_create(GRect(
    (window_bounds.size.w / 2) - (bitmap_bounds.size.w / 2) - (ACTION_BAR_WIDTH / 2),
    5,
    bitmap_bounds.size.w,
    bitmap_bounds.size.h));
  bitmap_layer_set_bitmap(s_icon_layer, s_icon_bitmap);
  bitmap_layer_set_compositing_mode(s_icon_layer, GCompOpSet);
  layer_add_child(window_layer, bitmap_layer_get_layer(s_icon_layer));

  // Add some text below the icon
  s_label_layer = text_layer_create(GRect(
    0,
    5 + bitmap_bounds.size.h + 5,
    window_bounds.size.w - ACTION_BAR_WIDTH,
    window_bounds.size.h));
  text_layer_set_overflow_mode(s_label_layer, GTextOverflowModeWordWrap);
  text_layer_set_text_alignment(s_label_layer, GTextAlignmentCenter);
  text_layer_set_font(s_label_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  snprintf(s_text_buffer, sizeof(s_text_buffer),
    "Continue %s for %d more minutes?",
    (current_countdown_type == COUNTDOWN_TYPE_FOCUS ? "focus" : "break"),
    (current_countdown_type == COUNTDOWN_TYPE_FOCUS ? focus_extra_time : break_extra_time)/60);
  text_layer_set_text(s_label_layer, s_text_buffer);
  layer_add_child(window_layer, text_layer_get_layer(s_label_layer));

  // Vibrate to alert of the choice
  vibes_double_pulse();
  // Initialize the icons for the action bar
  s_tick_bitmap = gbitmap_create_with_resource(RESOURCE_ID_TICK);
  s_cross_bitmap = gbitmap_create_with_resource(RESOURCE_ID_CROSS);
  // Initialize the action bar and add it to the window
  action_bar = action_bar_layer_create();
  action_bar_layer_set_click_config_provider(action_bar, break_or_extend_click_config_provider);
  action_bar_layer_set_icon(
    action_bar,
    BUTTON_ID_UP,
    s_tick_bitmap
  );
  action_bar_layer_set_icon(
    action_bar,
    BUTTON_ID_DOWN,
    s_cross_bitmap
  );
  action_bar_layer_add_to_window(action_bar, s_countdown_window);
  if(!window_is_loaded(s_countdown_window))
  {
    window_stack_push(s_countdown_window, true);
    current_window = WINDOW_COUNTDOWN;
  }
}

static void break_or_extend_click_config_provider(void *context)
{
  APP_LOG(APP_LOG_LEVEL_DEBUG, "BoE Click Config");
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Current Countdown Type %d", current_countdown_type);
  if(current_countdown_type == COUNTDOWN_TYPE_FOCUS)
  {
    window_single_click_subscribe(BUTTON_ID_UP, (ClickHandler) extend_focus);
    window_single_click_subscribe(BUTTON_ID_DOWN, (ClickHandler) start_break);
  }
  else
  {
    window_single_click_subscribe(BUTTON_ID_UP, (ClickHandler) extend_break);
    window_single_click_subscribe(BUTTON_ID_DOWN, (ClickHandler) start_focus);
  }
}

int main(void) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "main");
  init();
  app_event_loop();
  deinit();
}
