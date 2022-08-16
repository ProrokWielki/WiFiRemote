#include <Arduino.h>
#include <ArduinoOTA.h>
#include <ESP8266WiFi.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>

#include "esp_mqtt_helpers/config/wifi_config.h"

#include "esp_mqtt_helpers/helpers/ota.h"
#include "esp_mqtt_helpers/helpers/utils.h"

WiFiClient wifi_;

IRsend irsend(5);

void setup() {
  ota::initialize(wifi::hostname);

  WiFi.begin(wifi::ssid, wifi::password);

  wait_for_initialization([]() { return not WiFi.status() == WL_CONNECTED; });

  irsend.begin();
}

void loop() {
  ota::handle();

  irsend.sendNEC(0x20DF10EF, 32);

  delay(10000);
}
