"""
SENSA-NOISE AI Module
---------------------

Model Evaluation Script
Purpose: Evaluate trained Decision Tree classifier for environmental noise levels.

Classes:
0 - Silence
1 - Low
2 - Medium
3 - High

Author: Group 2
Course: Internet of Things
"""

import pandas as pd
import joblib
import matplotlib.pyplot as plt

from sklearn.metrics import accuracy_score, classification_report, confusion_matrix
from sklearn.tree import plot_tree


# ==============================
# Configuration
# ==============================

DATASET_PATH = "dataset.csv"
MODEL_PATH = "model.pkl"


# ==============================
# Load Model
# ==============================

print("Loading trained model...")

model = joblib.load(MODEL_PATH)

print("Model loaded successfully.")


# ==============================
# Load Dataset
# ==============================

print("Loading dataset...")

data = pd.read_csv(DATASET_PATH)

if "rms" not in data.columns or "noise_level" not in data.columns:
    raise ValueError("Dataset must contain 'rms' and 'noise_level' columns.")

X = data[["rms"]]
y = data["noise_level"]

print(f"Dataset loaded. Total samples: {len(data)}")


# ==============================
# Evaluation
# ==============================

print("\nEvaluating model...")

y_pred = model.predict(X)

accuracy = accuracy_score(y, y_pred)

print("\nEvaluation Results")
print("-------------------")
print(f"Accuracy: {accuracy:.4f}\n")

print("Classification Report:")
print(classification_report(y, y_pred))

print("Confusion Matrix:")
print(confusion_matrix(y, y_pred))


# ==============================
# Visualize Decision Tree
# ==============================

print("\nGenerating decision tree visualization...")

plt.figure(figsize=(10, 6))
plot_tree(
    model,
    feature_names=["rms"],
    class_names=["Silence", "Low", "Medium", "High"],
    filled=True
)

plt.title("SENSA-NOISE Decision Tree")
plt.show()

print("Evaluation completed successfully.")
