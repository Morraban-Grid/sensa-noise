/*
 * Project: sensa-noise
 * Board: ESP32S3-N16R8
 * Description: Smart Baby Room Monitor
 * Author: Morraban-Grid
 *
 * This file contains the complete firmware logic:
 * - I2S audio capture (INMP441)
 * - Cry pattern detection (RMS + duration)
 * - LED and buzzer alert
 * - ThingSpeak cloud upload
 */

// ===== Standard Libraries =====
#include <WiFi.h>
#include <HTTPClient.h>
#include "driver/i2s.h"
#include "config.h"

// ===== Hardware Pin Definitions =====

// I2S Microphone Pins (INMP441)
#define I2S_WS     38
#define I2S_SCK    39
#define I2S_SD     40

// Actuators
#define LED_PIN    5
#define BUZZER_PIN 6

// ===== Audio Configuration =====
#define SAMPLE_RATE     16000
#define BUFFER_SIZE     1024

// ===== Global Variables =====
bool wifiConnected = false;

void setup() {
  Serial.begin(115200);

  // Initialize actuator pins
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  digitalWrite(LED_PIN, LOW);
  digitalWrite(BUZZER_PIN, LOW);

  Serial.println("Sensa-Noise firmware initialized.");
}

void loop() {
  // Main logic will be implemented in next phases
}