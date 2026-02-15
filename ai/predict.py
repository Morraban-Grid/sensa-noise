"""
SENSA-NOISE AI Module
---------------------

Prediction Script
Purpose: Predict environmental noise level from RMS value using trained model.

Classes:
0 - Silence
1 - Low
2 - Medium
3 - High

Author: Group 2
Course: Internet of Things
"""

import sys
import joblib
import numpy as np


# ==============================
# Configuration
# ==============================

MODEL_PATH = "model.pkl"

CLASS_LABELS = {
    0: "Silence",
    1: "Low",
    2: "Medium",
    3: "High"
}


# ==============================
# Input Validation
# ==============================

if len(sys.argv) != 2:
    print("Usage: python predict.py <rms_value>")
    sys.exit(1)

try:
    rms_value = float(sys.argv[1])
except ValueError:
    print("Error: RMS value must be a number.")
    sys.exit(1)


# ==============================
# Load Model
# ==============================

print("Loading trained model...")
model = joblib.load(MODEL_PATH)
print("Model loaded successfully.")


# ==============================
# Prediction
# ==============================

input_data = np.array([[rms_value]])

prediction = model.predict(input_data)[0]

print("\nPrediction Result")
print("-----------------")
print(f"Input RMS value: {rms_value}")
print(f"Predicted class: {prediction}")
print(f"Noise level: {CLASS_LABELS[prediction]}")
