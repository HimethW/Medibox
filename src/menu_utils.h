#ifndef MENU_UTILS_H
#define MENU_UTILS_H

#include "display_utils.h"
#include "time_utils.h"
#include "config.h"

extern int max_modes;
extern int alarm_options;
extern int alarm_stopping_options;
extern String modes[];
extern String alarm_modes[];
extern String alarm_stopping_modes[];
extern float UTC_OFFSET;

void go_to_menu();
void goto_alarm_menu(int alarm);
void goto_stopping_menu(int alarm);
void run_mode(int mode);
void set_timeZone();
void set_alarm(int alarm);
int wait_for_button_press();

#endif