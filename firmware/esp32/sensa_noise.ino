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
#include <math.h>

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

// ===== Detection State =====
unsigned long soundStartTime = 0;
bool soundActive = false;

// ===== Global Variables =====
bool wifiConnected = false;

// ===== I2S Initialization =====
void initI2S() {

  i2s_config_t i2s_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
    .sample_rate = SAMPLE_RATE,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT,
    .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
    .communication_format = I2S_COMM_FORMAT_STAND_I2S,
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count = 8,
    .dma_buf_len = 256,
    .use_apll = false,
    .tx_desc_auto_clear = false,
    .fixed_mclk = 0
  };

  i2s_pin_config_t pin_config = {
    .bck_io_num = I2S_SCK,
    .ws_io_num = I2S_WS,
    .data_out_num = I2S_PIN_NO_CHANGE,
    .data_in_num = I2S_SD
  };

  i2s_driver_install(I2S_PORT, &i2s_config, 0, NULL);
  i2s_set_pin(I2S_PORT, &pin_config);
  i2s_zero_dma_buffer(I2S_PORT);

  Serial.println("I2S microphone initialized.");
}

// ===== Audio Capture =====
size_t readAudioBuffer() {

  size_t bytesRead = 0;

  i2s_read(
    I2S_PORT,
    (void*)audioBuffer,
    sizeof(audioBuffer),
    &bytesRead,
    portMAX_DELAY
  );

  return bytesRead;
}

// ===== RMS Calculation =====
float computeRMS(int32_t* buffer, size_t size) {

  double sum = 0.0;

  for (size_t i = 0; i < size; i++) {
    // Convert raw 32-bit sample to float
    float sample = (float)buffer[i];

    // Accumulate squared value (energy)
    sum += sample * sample;
  }

  // Mean of squared samples
  double mean = sum / size;

  // Root Mean Square
  float rms = sqrt(mean);

  return rms;
}

// ===== Setup =====
void setup() {
  Serial.begin(115200);

  // Initialize actuator pins
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  digitalWrite(LED_PIN, LOW);
  digitalWrite(BUZZER_PIN, LOW);

  initI2S();

  Serial.println("Sensa-Noise firmware initialized.");
}

// ===== Main Loop =====
void loop() {

  size_t bytesRead = readAudioBuffer();

  if (bytesRead > 0) {

    float rms = computeRMS(audioBuffer, BUFFER_SIZE);

    if (rms > RMS_THRESHOLD) {

      if (!soundActive) {
        soundActive = true;
        soundStartTime = millis();
      }

      // Check duration
      if (millis() - soundStartTime > CRY_DURATION_THRESHOLD) {
        Serial.println("Cry event detected!");
      }

    } else {
      soundActive = false;
    }
  }
}