/**
  Start -> Focus Screen

  Main Menu:
    Start Focus   --> Focus Screen
    Start Break   --> Break Screen
    Settings      --> Settings

  Focus Screen
    Pause --> Pause
    Exit  --> Main Menu
    (end) --> Break or Extend

  Break Screen
    Pause --> Pause
    Exit  --> Main Menu
    (end) --> Break or Extend

  Break or Extend
    Break   --> Break Screen
    Extend  --> Focus Screen
    Main    --> Main Menu

  Settings
    Change focus time
    Change break time
    Change extension

  Windows:
    - Main Menu
    - Counter (includes Break or Extend)
    - Settings

*/
#ifndef PEBBLODORO_H
#define PEBBLODORO_H

#include "main_menu.h"
#include "settings.h"

#define STORAGE_VERSION_KEY 0
#define STORAGE_VERSION 1
#define FOCUS_TIME_KEY 1
#define BREAK_TIME_KEY 2
#define FOCUS_EXTRA_KEY 3
#define BREAK_EXTRA_KEY 4

#define COUNTDOWN_TYPE_FOCUS 0
#define COUNTDOWN_TYPE_BREAK 1
#define COUNTDOWN_NEW 0
#define COUNTDOWN_EXTRA_TIME 1

#define WINDOW_MAIN_MENU 0
#define WINDOW_COUNTDOWN 1
#define WINDOW_BREAK_OR_EXTEND 2
#define WINDOW_SETTINGS 3


char current_countdown_type;
int current_window;

void window_unload(Window *);
void start_focus(void);
void start_break(void);


#endif
