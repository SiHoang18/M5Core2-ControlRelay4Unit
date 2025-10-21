#include "WifiTask.h"
bool reconnect = false;
String WIFI_SSID = "";
String WIFI_PASSWORD = "";
void InitWiFi()
{
    xTaskCreate(wifi_task,"wifi_sta",4096,NULL,1,NULL);
}
void wifi_task(void *pvParameter){
    if (WIFI_SSID.isEmpty() && WIFI_PASSWORD.isEmpty())
    {
        vTaskDelete(NULL);
    }

    WiFi.mode(WIFI_STA);
    if(WIFI_PASSWORD.isEmpty())
        WiFi.begin(WIFI_SSID);
    else
        WiFi.begin(WIFI_SSID.c_str(), WIFI_PASSWORD.c_str());

    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 20)
    {
    #ifdef M5_CORE2
        M5.update();
    #endif
        attempts++;
        if (WiFi.status() == WL_CONNECT_FAILED) {
            Serial.println("WiFi connect failed: wrong password or AP not found.");
            vTaskDelete(NULL);
            return;
        }
        vTaskDelay(WIFI_DELAY_TIME / portTICK_PERIOD_MS);
    }

    String ipAddress = WiFi.localIP().toString();
    Serial.println("Connected to WiFi");
    Serial.println(ipAddress);
#ifdef M5_CORE2
    M5.Lcd.fillRect(0, 0, M5.Lcd.width(), 20, BLACK); 
    M5.Lcd.setCursor(5, 10);      
    M5.Lcd.setTextSize(2);    
    M5.Lcd.setTextColor(GREEN, BLACK); 
    M5.Lcd.printf("IP: %s", ipAddress.c_str());
#endif
    initServerWeb();

    while (true)
    {
    #ifdef M5_CORE2
        M5.update();
    #endif
        if (WiFi.status() != WL_CONNECTED)
        {
            if (!reconnect)
            {
                reconnect = true;
                WiFi.disconnect();
                WiFi.begin(WIFI_SSID.c_str(), WIFI_PASSWORD.c_str());
                Serial.println("Reconnecting to WiFi...");
            }
        }
        else if (reconnect)
        {
            reconnect = false;
        }
        vTaskDelay(WIFI_DELAY_TIME / portTICK_PERIOD_MS);
    }
}