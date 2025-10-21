#ifndef INC_TASKWIFI_H_
#define INC_TASKWIFI_H_

#include "globals.h"

extern String WIFI_SSID;
extern String WIFI_PASSWORD;

extern void InitWiFi();
void wifi_task(void *pvParameter);
#endif /* INC_TASKWIFI_H_ */