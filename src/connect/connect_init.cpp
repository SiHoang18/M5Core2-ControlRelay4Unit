#include "../src/connect/connect_init.h"
void init_connect(){
    if (!LittleFS.begin(true)) {
        Serial.println("SPIFFS Mount Failed");
        return;
    }
    InitWiFi();
    // initMQTT();
    // initESPNow();
}