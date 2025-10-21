#include "../src/connect/ServerWeb.h"

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

// WebSocket Event Handler
void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
  switch (type) {
    case WS_EVT_CONNECT:
      Serial.printf("WebSocket client connected: %u\n", client->id());
      break;
    case WS_EVT_DISCONNECT:
      Serial.printf("WebSocket client disconnected: %u\n", client->id());
      break;
    case WS_EVT_DATA:
      receiveData(arg, data, len);
      break;
    case WS_EVT_ERROR:
      Serial.printf("WebSocket error: %s\n", (char*)data);
      break;
  }
}

// Periodic Sensor Data Broadcast
void sendSensorData(void *pvParameters) {
  while (true) {
    StaticJsonDocument<256> doc;
    doc["relay_1"] = relay_state[0];
    doc["relay_2"] = relay_state[1];
    doc["relay_3"] = relay_state[2];
    doc["relay_4"] = relay_state[3];

    String json;
    serializeJson(doc, json);

    ws.textAll(json);
    Serial.println("Send JSON: " + json);

    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}

void receiveData(void *arg, uint8_t *data, size_t len) {
  StaticJsonDocument<256> doc;
  DeserializationError error = deserializeJson(doc, data, len);

  if (error) {
    Serial.print("Failed to parse JSON: ");
    Serial.println(error.c_str());
    return;
  }
  String msg = "";
  for (size_t i = 0; i < len; i++) {
    msg += (char)data[i];
  }
  Serial.println("WebSocket message: " + msg);
  for (int i = 0; i < 4; i++) {
    String key = "relay_" + String(i + 1);
    String newState = doc[key] | relay_state[i];

    if (newState != relay_state[i]) {
        relay_state[i] = newState;
        drawRelayState(i);
        if (newState == "ON") {
            relay.relayWrite(i , 1); 
        } else {
            relay.relayWrite(i , 0);
        }
    }
  }
}

void initServerWeb() {
  Serial.println("Starting web server...");

  ws.onEvent(onEvent);
  server.addHandler(&ws);
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/webserver/index.html", "text/html");
  });
  server.on("/main.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/webserver/main.css", "text/css");
  });
  server.on("/main.js", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/webserver/main.js", "application/javascript");
  });
  server.begin();
  xTaskCreate(sendSensorData, "sendSensorData", 2048, NULL, 1, NULL);
}
