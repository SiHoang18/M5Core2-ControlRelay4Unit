#include "../src/common/configServer.h"
AsyncWebServer serverConfig(80);
AsyncWebSocket wsConfig("/ws");

void receiveDataFromConfig(void *arg, uint8_t *data, size_t len) {
  JsonDocument doc;
  DeserializationError error = deserializeJson(doc, data, len);
  if (error) {
    Serial.print("JSON parse failed: ");
    Serial.println(error.c_str());
    return;
  }

  // Print the received JSON content
  Serial.println("Received JSON:");
  serializeJsonPretty(doc, Serial);
  Serial.println();

  File file = LittleFS.open("/config.dat", "w",true);
  if (!file) {
    Serial.println("Failed to open file for writing");
    return;
  }

  serializeJson(doc, file);
  file.close();
}

void onEventConfig(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
  if(type == WS_EVT_CONNECT) {
    Serial.printf("WebSocket client connected: %u\n", client->id());
  } else if(type == WS_EVT_DISCONNECT) {
    Serial.printf("WebSocket client disconnected: %u\n", client->id());
  } else if(type == WS_EVT_DATA) {
    receiveDataFromConfig(arg, data, len);
    wsConfig.textAll("{\"status\":\"OK\"}");
    delay(100);
    ESP.restart();
  } else if(type == WS_EVT_ERROR) {
    Serial.printf("WebSocket error: %s\n", (char*)data);
  }
}

void initConfigServer() {
  Serial.println("Starting ACP...");  
  // if(!LittleFS.begin()){
  //   Serial.println("Fail to mount LittleFS");
  //   return;
  // }
  wsConfig.onEvent(onEventConfig);
  serverConfig.addHandler(&wsConfig);
  serverConfig.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/config/index.html", "text/html");
  });
  serverConfig.on("/main.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/config/main.css", "text/css");
  });
  serverConfig.on("/main.js", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/config/main.js", "application/javascript");
  });
  serverConfig.on("/pics/board.png", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/config/pics/board.png", "image/png");
  });
  serverConfig.begin();
}