#ifndef GLOBALS_H
#define GLOBALS_H
#include <Arduino.h>
#include <M5Core2.h>
#include <Unit_4RELAY.h>
#include <WiFi.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>
#include <ArduinoJson.h>
#include <ElegantOTA.h>
//
#include <../src/connect/WifiTask.h>
#include <../src/connect/connect_init.h>
#include <../src/connect/ServerWeb.h>
#include <../src/common/configServer.h>
#include <../src/common/info.h>
#include <../src/device/device_init.h>
#include <../src/device/unit_4relay.h>
#include <../src/device/display.h>
#endif