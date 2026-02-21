# sensa-noise  
Smart Baby Room Monitor — Edge Audio Intelligence with ESP32-S3

## Overview
sensa-noise is an embedded IoT system designed to detect baby crying patterns in real time using audio signal processing at the edge.

The system captures audio using an INMP441 I2S digital microphone connected to an ESP32-S3, performs real-time signal analysis (RMS, energy, frequency band detection, duration thresholds), and triggers alerts when a cry-like acoustic pattern is detected.

Alerts are transmitted to the cloud using ThingSpeak, enabling remote monitoring and data visualization.

## Problem
Parents cannot always distinguish between normal background noise and a baby crying event, especially when in another room.

## Solution
An edge-based monitoring system that:
- Detects sound peaks
- Identifies cry-like frequency patterns (400–600 Hz typical band)
- Applies duration thresholds to avoid false positives
- Triggers local alarms (LED + buzzer)
- Sends alerts to ThingSpeak cloud

## Technologies Used
- ESP32-S3 (C firmware)
- INMP441 I2S Digital Microphone
- Real-time signal processing
- Python (AI / feature analysis / model experimentation)
- ThingSpeak (IoT Cloud)
- Modular firmware architecture

## Future Improvements
- Lightweight ML model deployed on ESP32
- Multi-class detection: Cry / Impact / Ambient Noise
- Mobile dashboard
- MQTT-based alert system
- Edge AI optimization

## Author
Fernando Maylle  
GitHub: Morraban-Grid