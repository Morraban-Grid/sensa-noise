"""
SENSA-NOISE AI Module
---------------------

Model: Decision Tree Classifier
Purpose: Environmental noise level classification based on RMS values.

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

from sklearn.tree import DecisionTreeClassifier
from sklearn.model_selection import train_test_split
from sklearn.metrics import accuracy_score, classification_report, confusion_matrix


# ==============================
# Configuration
# ==============================

DATASET_PATH = "dataset.csv"
MODEL_OUTPUT_PATH = "model.pkl"
TEST_SIZE = 0.2
RANDOM_STATE = 42
MAX_DEPTH = 3


# ==============================
# Load Dataset
# ==============================

print("Loading dataset...")

data = pd.read_csv(DATASET_PATH)

if "rms" not in data.columns or "noise_level" not in data.columns:
    raise ValueError("Dataset must contain 'rms' and 'noise_level' columns.")

X = data[["rms"]]
y = data["noise_level"]

print(f"Dataset loaded successfully.")
print(f"Total samples: {len(data)}")


# ==============================
# Train/Test Split
# ==============================

X_train, X_test, y_train, y_test = train_test_split(
    X,
    y,
    test_size=TEST_SIZE,
    random_state=RANDOM_STATE,
    stratify=y
)

print("Data split completed.")
print(f"Training samples: {len(X_train)}")
print(f"Testing samples: {len(X_test)}")


# ==============================
# Model Training
# ==============================

print("Training Decision Tree model...")

model = DecisionTreeClassifier(
    max_depth=MAX_DEPTH,
    random_state=RANDOM_STATE
)

model.fit(X_train, y_train)

print("Model training completed.")


# ==============================
# Model Evaluation
# ==============================

y_pred = model.predict(X_test)

accuracy = accuracy_score(y_test, y_pred)

print("\nModel Evaluation Results")
print("------------------------")
print(f"Accuracy: {accuracy:.4f}\n")

print("Classification Report:")
print(classification_report(y_test, y_pred))

print("Confusion Matrix:")
print(confusion_matrix(y_test, y_pred))


# ==============================
# Save Model
# ==============================

joblib.dump(model, MODEL_OUTPUT_PATH)

print(f"\nModel saved successfully at '{MODEL_OUTPUT_PATH}'")
print("Training pipeline completed.")
