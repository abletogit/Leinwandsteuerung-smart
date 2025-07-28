/*
 * Example
 *
 * If you encounter any issues:
 * - check the readme.md at https://github.com/sinricpro/esp8266-esp32-sdk/blob/master/README.md
 * - ensure all dependent libraries are installed
 * - see https://github.com/sinricpro/esp8266-esp32-sdk/blob/master/README.md#arduinoide
 * - see https://github.com/sinricpro/esp8266-esp32-sdk/blob/master/README.md#dependencies
 * - open serial monitor and check whats happening
 * - check full user documentation at https://sinricpro.github.io/esp8266-esp32-sdk
 * - visit https://github.com/sinricpro/esp8266-esp32-sdk/issues and check for existing issues or open a new one
 */

 // Custom devices requires SinricPro ESP8266/ESP32 SDK 2.9.6 or later

// Uncomment the following line to enable serial debug output

#define ENABLE_DEBUG

#ifdef ENABLE_DEBUG
  #define DEBUG_ESP_PORT Serial
  #define NODEBUG_WEBSOCKETS
  #define NDEBUG
#endif

#include <Arduino.h>
#ifdef ESP8266
  #include <ESP8266WiFi.h>
#endif
#ifdef ESP32
  #include <WiFi.h>
#endif

#include <SinricPro.h>
#include "Leinwand.h"

#define APP_KEY    ""
#define APP_SECRET ""
#define DEVICE_ID  ""

#define SSID       ""
#define PASS       ""

#define BAUD_RATE  11520

#define ESP32_RX 16
#define ESP32_TX 17

Leinwand &leinwand = SinricPro[DEVICE_ID];

/*************
 * Variables *
 ***********************************************
 * Global variables to store the device states *
 ***********************************************/

// RangeController
std::map<String, int> globalRangeValues;



/*************
 * Callbacks *
 *************/

// RangeController
bool onRangeValue(const String &deviceId, const String& instance, int &rangeValue) {
  Serial.printf("[Device: %s]:onRangeValue Value for \"%s\" changed to %d\r\n", deviceId.c_str(), instance.c_str(), rangeValue);
  globalRangeValues[instance] = rangeValue;
  Serial2.println(rangeValue);
  Serial.println(rangeValue);
  return true;
}

bool onAdjustRangeValue(const String &deviceId, const String& instance, int &valueDelta) {
  globalRangeValues[instance] += valueDelta;
  Serial.printf("[Device: %s]:onAdjustRangeValue Value for \"%s\" changed about %d to %d\r\n", deviceId.c_str(), instance.c_str(), valueDelta, globalRangeValues[instance]);
  globalRangeValues[instance] = valueDelta;
  Serial2.println(valueDelta);
  Serial.println(valueDelta);
  return true;
}


/**********
 * Events *
 *************************************************
 * Examples how to update the server status when *
 * you physically interact with your device or a *
 * sensor reading changes.                       *
 *************************************************/

// RangeController
void updateRangeValue(String instance, int value) {
  leinwand.sendRangeValueEvent(instance, value);
}

/********* 
 * Setup *
 *********/

void setupSinricPro() {

  // RangeController
  leinwand.onRangeValue("position", onRangeValue);
  leinwand.onAdjustRangeValue("position", onAdjustRangeValue);

  SinricPro.onConnected([]{ Serial.printf("[SinricPro]: Connected\r\n"); });
  SinricPro.onDisconnected([]{ Serial.printf("[SinricPro]: Disconnected\r\n"); });
  SinricPro.begin(APP_KEY, APP_SECRET);
};

void setupWiFi() {
  #if defined(ESP8266)
    WiFi.setSleepMode(WIFI_NONE_SLEEP); 
    WiFi.setAutoReconnect(true);
  #elif defined(ESP32)
    WiFi.setSleep(false); 
    WiFi.setAutoReconnect(true);
  #endif

  WiFi.begin(SSID, PASS);
  Serial.printf("[WiFi]: Connecting to %s", SSID);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.printf(".");
    delay(250);
  }
  Serial.printf("connected\r\n");
}

void setup() {
  Serial.begin(BAUD_RATE);
  setupWiFi();
  setupSinricPro();
  Serial.begin(115200);  // Serielle Konsole
 
  // UART2 starten: Baudrate 9600, RX=16, TX=17
  Serial2.begin(9600, SERIAL_8N1, 16, 17);
 
  Serial.println("UART2 ready");
}

/********
 * Loop *
 ********/

void loop() {
  SinricPro.handle();
}