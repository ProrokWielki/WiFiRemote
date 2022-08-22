#include <Arduino.h>
#include <ArduinoOTA.h>
#include <ESP8266WiFi.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <PubSubClient.h>

#include "esp_mqtt_helpers/config/mqtt_config.h"
#include "esp_mqtt_helpers/config/wifi_config.h"

#include "esp_mqtt_helpers/helpers/json.h"
#include "esp_mqtt_helpers/helpers/mqtt.h"
#include "esp_mqtt_helpers/helpers/ota.h"
#include "esp_mqtt_helpers/helpers/utils.h"

#include "devices/airwell_rc08b.h"

WiFiClient wifi_;
PubSubClient mqtt_(mqtt::server_ip, mqtt::server_port, wifi_);

IRsend irsend(5);

using namespace airwell::rc08b;

void handle_ac_controls(byte * string, unsigned int length)
{
    uint16_t raw_buffer[IR_FRAME_LENGTH];

    std::string raw_json(reinterpret_cast<char *>(string), length);
    auto parsed_json = simple_str_json_to_map(raw_json);

    get_buffer_to_send(raw_buffer, to_mode[parsed_json["mode"]], to_temperature[parsed_json["temperature"]], to_fan_speed[parsed_json["fan_speed"]],
                       to_turbo[parsed_json["turbo"]], to_swing[parsed_json["swing"]]);

    // it needs to be sent twice ...
    irsend.sendRaw(raw_buffer, IR_FRAME_LENGTH, IR_FREQUENCY);
    delay(5);
    irsend.sendRaw(raw_buffer, IR_FRAME_LENGTH, IR_FREQUENCY);
}

std::map<std::string, std::function<void(byte * payload, unsigned int length)>> handle_topic{{"AC_controls", handle_ac_controls}};

void setup()
{
    ota::initialize(wifi::hostname);

    WiFi.begin(wifi::ssid, wifi::password);

    wait_for_initialization([]() { return not WiFi.status() == WL_CONNECTED; });
    wait_for_initialization([]() { return not mqtt_.connect(mqtt::client_id); });

    mqtt_.setCallback([](char * topic, byte * payload, unsigned int length) { handle_topic[topic](payload, length); });
    mqtt::subscribe(mqtt_, handle_topic);

    irsend.begin();
}

void loop()
{
    ota::handle();
    mqtt_.loop();
}
