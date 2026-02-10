#ifndef CONFIG_H
#define CONFIG_H

/*************************************************
 *  SENSA-NOISE
 *  Smart Environmental Noise Sensing System
 *
 *  Archivo: config.h
 *  Descripción:
 *  Configuración global del proyecto:
 *  - WiFi
 *  - ThingSpeak
 *  - Pines de hardware
 *  - Parámetros de muestreo
 *************************************************/

 /* ===============================
   CONFIGURACIÓN WIFI
   =============================== */

// ⚠️ Reemplaza con tus datos reales
#define WIFI_SSID "MI_WIFI"
#define WIFI_PASSWORD "PASSWORD"

/* ===============================
   CONFIGURACIÓN THINGSPEAK
   =============================== */

// Canal ThingSpeak
#define THINGSPEAK_CHANNEL_ID  0000000   // <-- tu Channel ID

// Write API Key
#define THINGSPEAK_API_KEY     "TU_API_KEY"

// Servidor ThingSpeak
#define THINGSPEAK_SERVER      "api.thingspeak.com"

// Intervalo de envío (mínimo 15s en plan gratis)
#define THINGSPEAK_INTERVAL_MS 20000


/* ===============================
   CONFIGURACIÓN DE PINES ESP32-S3
   =============================== */

// --- Micrófono INMP441 (I2S) ---
#define I2S_SCK_PIN    17   // Serial Clock
#define I2S_WS_PIN     18   // Word Select
#define I2S_SD_PIN     16   // Serial Data
#define I2S_LR_PIN     -1   // L/R fijado a GND físicamente

// --- Actuadores ---
#define LED_PIN        4
#define BUZZER_PIN     5


/* ===============================
   CONFIGURACIÓN DE AUDIO
   =============================== */

// Frecuencia de muestreo (Hz)
#define SAMPLE_RATE        16000

// Tamaño del buffer de lectura
#define AUDIO_BUFFER_SIZE  512


/* ===============================
   CLASIFICACIÓN DE RUIDO
   =============================== */

// Umbrales de RMS (valores empíricos, ajustables)
#define NOISE_SILENCE_MAX      500
#define NOISE_LOW_MAX         2000
#define NOISE_MEDIUM_MAX      5000

// Estados de ruido
typedef enum{
    NOISE_SILENCE = 0,
    NOISE_LOW,
    NOISE_MEDIUM,
    NOISE_HIGH

} noise_level_t;

#endif //CONFIG_H
