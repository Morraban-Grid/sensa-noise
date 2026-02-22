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

// ===== Debug Control =====
#define DEBUG_MODE true   // Set to false for production

#if DEBUG_MODE
  #define DEBUG_PRINT(x)    Serial.print(x)
  #define DEBUG_PRINTLN(x)  Serial.println(x)
#else
  #define DEBUG_PRINT(x)
  #define DEBUG_PRINTLN(x)
#endif

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

  DEBUG_PRINTLN("I2S microphone initialized.");
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

    int32_t sample = buffer[i] >> 8;  // Normalize 24-bit data
    float fsample = (float)sample;
    sum += fsample * fsample;
  }

  double mean = sum / size;
  return sqrt(mean);
}

// ===== Alert Trigger =====
void triggerAlert() {

  DEBUG_PRINTLN("Triggering alert...");

  digitalWrite(LED_PIN, HIGH);
  digitalWrite(BUZZER_PIN, HIGH);

  delay(2000);

  digitalWrite(LED_PIN, LOW);
  digitalWrite(BUZZER_PIN, LOW);

  DEBUG_PRINTLN("Alert finished.");
}

// ===== WiFi Connection =====
bool connectWiFi() {

  DEBUG_PRINTLN("Connecting to WiFi...");

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  unsigned long startAttemptTime = millis();
  const unsigned long timeout = 15000;

  while (WiFi.status() != WL_CONNECTED &&
         millis() - startAttemptTime < timeout) {
    delay(500);
    DEBUG_PRINT(".");
  }

  if (WiFi.status() == WL_CONNECTED) {
    DEBUG_PRINTLN("\nWiFi connected.");
    DEBUG_PRINT("IP address: ");
    DEBUG_PRINTLN(WiFi.localIP());
    return true;
  } else {
    DEBUG_PRINTLN("\nWiFi connection FAILED.");
    WiFi.disconnect(true);
    return false;
  }
}

// ===== ThingSpeak Upload =====
bool sendToThingSpeak(int value) {

  if (WiFi.status() != WL_CONNECTED) {
    DEBUG_PRINTLN("WiFi not connected. Skipping upload.");
    return false;
  }

  HTTPClient http;

  String url = String(THINGSPEAK_HOST) +
               "/update?api_key=" +
               THINGSPEAK_API_KEY +
               "&field1=" +
               String(value);

  http.begin(url);
  int httpCode = http.GET();

  if (httpCode == 200) {
    DEBUG_PRINTLN("ThingSpeak upload successful.");
    http.end();
    return true;
  } else {
    DEBUG_PRINT("ThingSpeak upload failed. HTTP code: ");
    DEBUG_PRINTLN(httpCode);
    http.end();
    return false;
  }
}

// ===== Setup =====
void setup() {

  Serial.begin(115200);

  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  digitalWrite(LED_PIN, LOW);
  digitalWrite(BUZZER_PIN, LOW);

  initI2S();

  if (!connectWiFi()) {
    DEBUG_PRINTLN("System will continue without cloud connectivity.");
  }

  DEBUG_PRINTLN("Sensa-Noise firmware initialized.");
}

// ===== Main Loop =====
void loop() {

  size_t samplesRead = 0;

  readAudioBuffer(&samplesRead);

  if (samplesRead == 0) {
    return;
  }

  float rms = computeRMS(audioBuffer, samplesRead);

  DEBUG_PRINT("RMS: ");
  DEBUG_PRINTLN(rms);

  if (rms > RMS_THRESHOLD) {

    if (!soundActive) {
      soundActive = true;
      soundStartTime = millis();
    }

    unsigned long elapsed = millis() - soundStartTime;

    if (elapsed > CRY_DURATION_THRESHOLD) {

      DEBUG_PRINTLN("Cry event detected.");

      triggerAlert();
      sendToThingSpeak(1);

      soundActive = false;
    }

  } else {
    soundActive = false;
  }
}