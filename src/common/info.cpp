#include <../src/common/info.h>

void printOnM5(const char* text, uint16_t color, uint8_t textSize, uint8_t x, uint8_t y) {
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setCursor(x,y);
    M5.Lcd.setTextSize(textSize);
    M5.Lcd.setTextColor(color);
    M5.Lcd.println(text);
}
DynamicJsonDocument json_decode(File file){
    if(!file) return NULL;
    size_t size = file.size();
   if (size == 0) {
        Serial.println("Config file is empty");
        return NULL;
    }
    std::unique_ptr<char[]> buf(new char[size + 1]);
    file.readBytes(buf.get(), size);
    buf[size] = '\0';

    DynamicJsonDocument doc(1024);
    DeserializationError error = deserializeJson(doc, buf.get());
    if (error) {
        Serial.print("Failed to parse config file: ");
        Serial.println(error.c_str());
        return NULL;
    }
    return doc;
}

void load_config(){
    if(!LittleFS.exists("/config.dat")){
        return;
    }
    File configFile = LittleFS.open("/config.dat","r",false);
    if (!configFile) {
        Serial.println("Failed to open config file");
        return;
    }
    DynamicJsonDocument doc(1024);
    doc = json_decode(configFile);
    if(doc == NULL){
        Serial.println("File is empty");
        configFile.close();
        // return;
    }
    WIFI_SSID = doc["WiFi SSID"].as<String>();
    WIFI_PASSWORD = doc["WiFi Password"].as<String>();
    configFile.close();
}

void initWiFiACP(){
    WiFi.mode(WIFI_AP);
    WiFi.softAP(WIFI_ACP_NAME);
    Serial.print("Wifi ACP Address: ");
    Serial.println(WiFi.softAPIP().toString());
    initConfigServer();
#ifdef M5_CORE2
    M5.Lcd.setCursor(0, 20);
    M5.Lcd.println("Please connect to:");
    M5.Lcd.setTextColor(CYAN);

    int screenWidth = M5.Lcd.width();
    int textWidth = M5.Lcd.textWidth(String(WIFI_ACP_NAME));
    M5.Lcd.setCursor((screenWidth - textWidth) / 2, 20);
    M5.Lcd.println(WIFI_ACP_NAME);

    M5.Lcd.setTextColor(WHITE);
    M5.Lcd.setCursor(0, 40);
    M5.Lcd.println("Scan QR to visit:");
    String ipAddress = WiFi.softAPIP().toString();
    textWidth = M5.Lcd.textWidth(ipAddress);
    M5.Lcd.setTextColor(YELLOW);
    M5.Lcd.setCursor((screenWidth - textWidth) / 2, 40);
    M5.Lcd.println(ipAddress);
    String url = "http://" + ipAddress;
    int qrSize = 150;  
    int qrX = (screenWidth - qrSize) / 2;
    int qrY = 70;
    M5.Lcd.qrcode(url.c_str(), qrX, qrY, qrSize);
#endif
}

void task_reset_esp(void *pvParameter) {
    unsigned long buttonPressStart = 0;

    while (true) {
    #ifdef M5_CORE2
        M5.update();
    #endif
        #ifdef M5_CORE2
        if (M5.BtnB.wasPressed()) {
            buttonPressStart = millis();   // ghi lại thời điểm bắt đầu nhấn
        }

        if (M5.BtnB.isPressed() && buttonPressStart > 0) {
            if (millis() - buttonPressStart > 5000) {
                if (LittleFS.exists("/config.dat")) {
                    LittleFS.remove("/config.dat");
                }
                printOnM5("M5Core2 Restart", WHITE, 3, 25, 100);
                delay(1000);
                ESP.restart();
            }
        }

        if (M5.BtnB.wasReleased()) {
            buttonPressStart = 0;   
        }
        #endif
        // else{
        //     buttonPressStart = 0;
        // }
        vTaskDelay(RESET_DELAY_TIME);
    }
}
void reset_esp(){
    xTaskCreate(task_reset_esp,"reset_task",4096,NULL,1,NULL);
}
bool check_config(){
    load_config();
    reset_esp();
    if(WIFI_SSID.isEmpty()){
        initWiFiACP();
        return false;
    }
    return true;
}