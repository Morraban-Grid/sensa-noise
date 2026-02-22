"""
Project: sensa-noise
Module: Audio Feature Extraction Utilities

Responsibilities:
- Simulate RMS computation
- Calculate signal energy
- Extract duration above threshold
"""

import numpy as np


def compute_rms(signal: np.ndarray) -> float:
    if len(signal) == 0:
        return 0.0
    return np.sqrt(np.mean(np.square(signal)))


def compute_energy(signal: np.ndarray) -> float:
    return np.sum(np.square(signal))


def extract_duration_above_threshold(signal: np.ndarray,
                                     threshold: float,
                                     sample_rate: int) -> float:
    above = np.abs(signal) > threshold
    duration_samples = np.sum(above)
    return duration_samples / sample_rate


if __name__ == "__main__":
    # Simulated audio signal
    sample_rate = 16000
    duration_seconds = 2
    t = np.linspace(0, duration_seconds, sample_rate * duration_seconds)

    simulated_signal = 1000 * np.sin(2 * np.pi * 440 * t)

    rms = compute_rms(simulated_signal)
    energy = compute_energy(simulated_signal)
    duration = extract_duration_above_threshold(simulated_signal, 500, sample_rate)

    print(f"RMS: {rms}")
    print(f"Energy: {energy}")
    print(f"Duration above threshold: {duration} seconds")