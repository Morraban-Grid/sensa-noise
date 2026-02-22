# System Architecture — Sensa-Noise

## 1. Overview

Sensa-Noise is an IoT-based cry detection system designed using a three-layer architecture:

- Edge Layer (Embedded System)
- Cloud Layer (Data & Telemetry)
- AI Layer (Analytics & Optimization)

This separation ensures modularity, scalability, and future extensibility.

---

# 2. Edge Layer (Embedded Layer)

## 2.1 Hardware Components

- ESP32S3-N16R8
- INMP441 I2S Digital Microphone
- LED (GPIO 5, 220Ω resistor)
- Active 3V Buzzer (GPIO 6)

### I2S Pin Mapping

| Component | GPIO |
|-----------|------|
| WS        | 38   |
| SCK       | 39   |
| SD        | 40   |

⚠ INMP441 operates strictly at 3.3V.

Forbidden GPIOs:
0, 3, 8, 9, 45, 46

---

## 2.2 Responsibilities

The Edge Layer performs:

1. Real-time audio acquisition via I2S
2. RMS computation
3. Duration-based cry detection
4. Local alert activation (LED + Buzzer)
5. Cloud event transmission (ThingSpeak)

All signal processing occurs locally to minimize latency.

---

# 3. Cloud Layer

## 3.1 Platform

ThingSpeak is used for:

- Event storage
- Telemetry logging
- Remote monitoring

Data flow:

ESP32 → HTTP GET → ThingSpeak API → Channel storage

Each cry event sends:

field1 = 1

---

## 3.2 Responsibilities

- Persistent data storage
- Historical visualization
- Dataset generation for AI layer

Security considerations:

- API keys stored in config.h
- config.h excluded via .gitignore
- No credentials committed to repository

---

# 4. AI Layer

The AI layer is implemented in Python.

Modules:

- analyze_thingspeak_data.py
- feature_extraction.py
- optimize_thresholds.py

Responsibilities:

1. Download dataset from ThingSpeak
2. Convert JSON to DataFrame
3. Extract signal features (RMS, energy, duration)
4. Optimize detection thresholds

This layer enables continuous improvement of detection accuracy.

---

# 5. Data Flow Summary

INMP441  
→ I2S Sampling (16 kHz)  
→ RMS Computation  
→ Duration Logic  
→ Alert Trigger  
→ ThingSpeak Upload  
→ Python Analysis  
→ Threshold Optimization  

---

# 6. Scalability Considerations

Future improvements:

- FFT-based frequency analysis
- Machine learning classifier
- MQTT instead of HTTP
- Edge TinyML model deployment
- Mobile application integration

---

# 7. Architectural Benefits

- Clear separation of concerns
- Edge-first processing
- Cloud-assisted analytics
- AI-driven optimization
- Secure credential handling

---

Sensa-Noise follows a modern IoT + AI hybrid architecture suitable for academic and production-level development.