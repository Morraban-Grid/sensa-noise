/*
 * Project: sensa-noise
 * Board: ESP32S3-N16R8
 * Author: Morraban-Grid
 *
 * Firmware responsibilities:
 * - I2S audio capture (INMP441)
 * - Cry detection (RMS + duration)
 * - LED + buzzer alert
 * - ThingSpeak upload
 */

// ===== Required Libraries =====
#include <WiFi.h>
#include <HTTPClient.h>
#include "driver/i2s.h"
#include "config.h"

// ===== Hardware Pin Definitions =====

// INMP441 I2S Microphone
#define I2S_WS     38   // Word Select
#define I2S_SCK    39   // Serial Clock
#define I2S_SD     40   // Serial Data (from mic)

// Actuators
#define LED_PIN    5
#define BUZZER_PIN 6

// ===== Audio Configuration =====
#define SAMPLE_RATE     16000      // 16 kHz
#define BUFFER_SIZE     1024       // Samples per read
#define I2S_PORT        I2S_NUM_0

// ===== Detection Thresholds (initial values) =====
#define RMS_THRESHOLD           800000.0
#define CRY_DURATION_THRESHOLD  1500     // milliseconds

// ===== Global Buffers =====
int32_t audioBuffer[BUFFER_SIZE];

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