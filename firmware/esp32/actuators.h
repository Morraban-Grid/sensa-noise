#ifndef ACTUATORS_H
#define ACTUATORS_H

/*************************************************
 *  actuators.h
 *
 *  Handles system actuators:
 *  - Status LED
 *  - Active buzzer
 *  Reacts to classified noise levels
 *************************************************/

#include <Arduino.h>
#include "config.h"


/* ===============================
   INITIALIZATION
   =============================== */

/**
 * @brief Initialize LED and buzzer pins
 */
inline void actuators_init() {
    pinMode(LED_PIN, OUTPUT);
    pinMode(BUZZER_PIN, OUTPUT);

    digitalWrite(LED_PIN, LOW);
    digitalWrite(BUZZER_PIN, LOW);
}


/* ===============================
   ACTUATOR CONTROL
   =============================== */

/**
 * @brief Update actuators based on noise level
 * @param level Classified noise level
 */
inline void actuators_update(noise_level_t level) {
    switch (level) {

        case NOISE_SILENCE:
            // Silence: everything off
            digitalWrite(LED_PIN, LOW);
            digitalWrite(BUZZER_PIN, LOW);
            break;

        case NOISE_LOW:
            // Low noise: LED on, buzzer off
            digitalWrite(LED_PIN, HIGH);
            digitalWrite(BUZZER_PIN, LOW);
            break;

        case NOISE_MEDIUM:
            // Medium noise: LED on, short buzzer alert
            digitalWrite(LED_PIN, HIGH);
            digitalWrite(BUZZER_PIN, HIGH);
            delay(100);
            digitalWrite(BUZZER_PIN, LOW);
            break;

        case NOISE_HIGH:
            // High noise: LED on, continuous buzzer
            digitalWrite(LED_PIN, HIGH);
            digitalWrite(BUZZER_PIN, HIGH);
            break;

        default:
            // Failsafe
            digitalWrite(LED_PIN, LOW);
            digitalWrite(BUZZER_PIN, LOW);
            break;
    }
}

#endif // ACTUATORS_H
