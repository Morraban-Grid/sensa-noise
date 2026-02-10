#ifndef NOISE_SENSOR_H
#define NOISE_SENSOR_H

/*************************************************
 *  noise_sensor.h
 *
 *  Handles INMP441 microphone using I2S:
 *  - I2S initialization
 *  - Audio sampling
 *  - RMS calculation
 *  - Noise level classification
 *************************************************/

#include <Arduino.h>
#include "driver/i2s.h"
#include "config.h"


/* ===============================
   I2S CONFIGURATION
   =============================== */

static const i2s_config_t i2s_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
    .sample_rate = SAMPLE_RATE,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
    .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
    .communication_format = I2S_COMM_FORMAT_I2S,
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count = 4,
    .dma_buf_len = AUDIO_BUFFER_SIZE,
    .use_apll = false,
    .tx_desc_auto_clear = false,
    .fixed_mclk = 0
};

static const i2s_pin_config_t i2s_pin_config = {
    .bck_io_num   = I2S_SCK_PIN,
    .ws_io_num    = I2S_WS_PIN,
    .data_out_num = I2S_PIN_NO_CHANGE,
    .data_in_num  = I2S_SD_PIN
};


/* ===============================
   FUNCTION DECLARATIONS
   =============================== */

/**
 * @brief Initialize I2S microphone
 */
inline void noise_sensor_init() {
    i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
    i2s_set_pin(I2S_NUM_0, &i2s_pin_config);
    i2s_zero_dma_buffer(I2S_NUM_0);
}


/**
 * @brief Read audio samples and compute RMS value
 * @return RMS amplitude
 */
inline uint32_t noise_sensor_read_rms() {
    int16_t audio_buffer[AUDIO_BUFFER_SIZE];
    size_t bytes_read = 0;

    i2s_read(
        I2S_NUM_0,
        (void*)audio_buffer,
        sizeof(audio_buffer),
        &bytes_read,
        portMAX_DELAY
    );

    uint32_t sum = 0;
    uint32_t samples = bytes_read / sizeof(int16_t);

    for (uint32_t i = 0; i < samples; i++) {
        int32_t sample = audio_buffer[i];
        sum += sample * sample;
    }

    uint32_t rms = sqrt((double)sum / samples);
    return rms;
}


/**
 * @brief Classify noise level based on RMS value
 * @param rms RMS amplitude
 * @return noise_level_t
 */
inline noise_level_t noise_sensor_classify(uint32_t rms) {
    if (rms < NOISE_SILENCE_MAX) {
        return NOISE_SILENCE;
    } else if (rms < NOISE_LOW_MAX) {
        return NOISE_LOW;
    } else if (rms < NOISE_MEDIUM_MAX) {
        return NOISE_MEDIUM;
    } else {
        return NOISE_HIGH;
    }
}

#endif // NOISE_SENSOR_H
