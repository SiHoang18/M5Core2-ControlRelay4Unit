#ifndef INFO_H
#define INFO_H

#include "globals.h"

extern void initWiFiACP();
extern bool check_config();
DynamicJsonDocument json_decode(File file);
void load_config();
void reset_esp();
void task_reset_esp(void *pvParameter);

#endif