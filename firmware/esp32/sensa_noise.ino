/*
 * Project: sensa-noise
 * Board: ESP32S3-N16R8
 * Author: Morraban-Grid
 *
 * Firmware responsibilities:
 * - I2S audio capture (INMP441)
 * - Cry detection (RMS + duration)
 * - LED + buzzer alert
 * - ThingSpeak upload (future phase)
 */

// ===== Required Libraries =====
#include <WiFi.h>
#include <HTTPClient.h>
#include "driver/i2s.h"
#include "config.h"
#include <math.h>

// ===== Hardware Pin Definitions =====

// INMP441 I2S Microphone
#define I2S_WS     38
#define I2S_SCK    39
#define I2S_SD     40

// Actuators
#define LED_PIN    5
#define BUZZER_PIN 6

// ===== Audio Configuration =====
#define SAMPLE_RATE     16000
#define BUFFER_SIZE     1024
#define I2S_PORT        I2S_NUM_0

// ===== Detection Thresholds =====
#define RMS_THRESHOLD           800000.0
#define CRY_DURATION_THRESHOLD  1500  // milliseconds

// ===== Global Buffers =====
int32_t audioBuffer[BUFFER_SIZE];

// ===== Detection State =====
unsigned long soundStartTime = 0;
bool soundActive = false;

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
size_t readAudioBuffer(size_t* samplesRead) {

  size_t bytesRead = 0;

  i2s_read(
    I2S_PORT,
    (void*)audioBuffer,
    sizeof(audioBuffer),
    &bytesRead,
    portMAX_DELAY
  );

  *samplesRead = bytesRead / sizeof(int32_t);

  return bytesRead;
}

// ===== RMS Calculation =====
float computeRMS(int32_t* buffer, size_t size) {

  if (size == 0) return 0.0;

  double sum = 0.0;

  for (size_t i = 0; i < size; i++) {

    // INMP441 provides 24-bit data in 32-bit container
    int32_t sample = buffer[i] >> 8;  // normalize

    float fsample = (float)sample;
    sum += fsample * fsample;
  }

  double mean = sum / size;
  return sqrt(mean);
}

// ===== Alert Trigger =====
void triggerAlert() {

  Serial.println("Triggering alert...");

  digitalWrite(LED_PIN, HIGH);
  digitalWrite(BUZZER_PIN, HIGH);

  delay(2000);

  digitalWrite(LED_PIN, LOW);
  digitalWrite(BUZZER_PIN, LOW);

  Serial.println("Alert finished.");
}

// ===== Setup =====
void setup() {

  Serial.begin(115200);

  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  digitalWrite(LED_PIN, LOW);
  digitalWrite(BUZZER_PIN, LOW);

  initI2S();

  Serial.println("Sensa-Noise firmware initialized.");
}

// ===== Main Loop =====
void loop() {

  size_t samplesRead = 0;
  readAudioBuffer(&samplesRead);

  if (samplesRead > 0) {

    float rms = computeRMS(audioBuffer, samplesRead);

    if (rms > RMS_THRESHOLD) {

      if (!soundActive) {
        soundActive = true;
        soundStartTime = millis();
      }

      if (millis() - soundStartTime > CRY_DURATION_THRESHOLD) {
        Serial.println("Cry event detected!");
        triggerAlert();
        soundActive = false;
      }

    } else {
      soundActive = false;
    }
  }
}