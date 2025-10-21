#ifndef INC_SERVERWEB_H_
#define INC_SERVERWEB_H_
#include <globals.h>  

extern void initServerWeb();
void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len);
void sendSensorData(void *pvParameters);
void receiveData(void *arg, uint8_t *data, size_t len);

#endif
