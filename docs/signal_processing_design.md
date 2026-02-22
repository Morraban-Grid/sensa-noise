# Signal Processing Design — Sensa-Noise

## 1. Overview

The cry detection algorithm is based on energy estimation using Root Mean Square (RMS) combined with duration-based logic.

This approach ensures:

- Low computational cost
- Real-time performance
- Stability against short noise spikes

---

# 2. RMS Calculation

## 2.1 Formula

For a signal x[n] of N samples:

RMS = sqrt( (1/N) * Σ x[n]^2 )

Where:

- x[n] = audio sample
- N = number of samples
- sqrt = square root

---

## 2.2 Implementation Details

- Sampling rate: 16 kHz
- Buffer size: 1024 samples
- 24-bit audio normalized from 32-bit container
- Computation uses double precision accumulation

RMS estimates signal energy level.

---

# 3. Duration-Based Detection

RMS threshold alone is insufficient because short spikes may trigger false positives.

Therefore, detection logic is:

IF RMS > threshold  
AND duration_above_threshold > 1500 ms  
THEN cry_detected = true  

This ensures:

- Noise spikes are ignored
- Sustained sound events are detected

---

# 4. Threshold Selection

Initial RMS threshold:

800000.0

Duration threshold:

1500 milliseconds

These values can be optimized using historical dataset analysis in the AI layer.

---

# 5. Future FFT Expansion

To improve detection accuracy, Fast Fourier Transform (FFT) may be implemented.

## 5.1 Why FFT?

Baby cries typically have dominant frequency components between:

300 Hz – 600 Hz

FFT would allow:

- Frequency band analysis
- Spectral energy detection
- More robust classification

---

## 5.2 Proposed Extension

Pipeline:

Audio → RMS Filter  
→ FFT  
→ Band Energy Extraction  
→ Feature Vector  
→ ML Classifier  

Possible future classifiers:

- Logistic Regression
- SVM
- Lightweight Neural Network (TinyML)

---

# 6. Computational Efficiency

Current RMS method:

- O(N)
- Real-time capable on ESP32
- Minimal memory footprint

FFT method:

- O(N log N)
- Higher accuracy
- Slightly higher CPU usage

---

# 7. Design Philosophy

The current design prioritizes:

- Simplicity
- Deterministic behavior
- Low power consumption
- Edge autonomy

Signal processing is designed to scale from rule-based detection to ML-based inference without major architectural changes.

---

Sensa-Noise is built to evolve from a deterministic DSP system into an intelligent edge AI system.