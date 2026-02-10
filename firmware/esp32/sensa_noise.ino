/*************************************************
 *  SENSA-NOISE
 *  Smart Environmental Noise Sensing System
 *
 *  File: sensa_noise.ino
 *  Description:
 *  Main firmware logic for ESP32-S3
 *************************************************/

#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>

#include "config.h"
#include "noise_sensor.h"
#include "actuators.h"


/* ===============================
   WIFI CONNECTION
   =============================== */

void connect_wifi() {
    Serial.print("Connecting to WiFi");

    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("\nWiFi connected");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
}


/* ===============================
   THINGSPEAK COMMUNICATION
   =============================== */

void send_to_thingspeak(uint32_t rms, noise_level_t level) {
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("WiFi not connected, skipping upload");
        return;
    }

    HTTPClient http;

    String url = "http://";
    url += THINGSPEAK_SERVER;
    url += "/update?api_key=";
    url += THINGSPEAK_API_KEY;
    url += "&field1=";
    url += String(rms);
    url += "&field2=";
    url += String((int)level);

    http.begin(url);
    int http_code = http.GET();

    if (http_code > 0) {
        Serial.print("ThingSpeak response: ");
        Serial.println(http_code);
    } else {
        Serial.print("ThingSpeak error: ");
        Serial.println(http.errorToString(http_code));
    }

    http.end();
}


/* ===============================
   ARDUINO SETUP
   =============================== */

void setup() {
    Serial.begin(115200);
    delay(1000);

    Serial.println("Starting SENSA-NOISE firmware");

    connect_wifi();
    noise_sensor_init();
    actuators_init();

    Serial.println("System initialized");
}


/* ===============================
   ARDUINO LOOP
   =============================== */

void loop() {
    uint32_t rms = noise_sensor_read_rms();
    noise_level_t level = noise_sensor_classify(rms);

    actuators_update(level);

    Serial.print("RMS: ");
    Serial.print(rms);
    Serial.print(" | Noise level: ");
    Serial.println((int)level);

    send_to_thingspeak(rms, level);

    delay(THINGSPEAK_INTERVAL_MS);
}
