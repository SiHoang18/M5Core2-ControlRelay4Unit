#include <globals.h>

void setup() {
  Serial.begin(115200);
  M5.begin(true,false,true,true);
  if (!LittleFS.begin(true)) {
      Serial.println("[ERROR] LittleFS Mount Failed!");
      return;
  }
  if(check_config()){
    M5.Lcd.clear(BLACK);
    init_connect();
    init_device();
  }
}

void loop() {

}
