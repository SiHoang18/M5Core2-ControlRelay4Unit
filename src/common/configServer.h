#ifndef INC_CONFIG_SERVER_H
#define INC_CONFIG_SERVER_H
#include "globals.h"
extern void initConfigServer();
void onEventConfig(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len);
void receiveDataFromConfig(void *arg, uint8_t *data, size_t len);
#endif