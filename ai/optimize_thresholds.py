"""
Project: sensa-noise
Module: Threshold Optimization Tool

Responsibilities:
- Analyze dataset
- Suggest optimal RMS threshold
"""

import pandas as pd
import numpy as np
import sys


def suggest_threshold(df: pd.DataFrame) -> float:
    if df.empty:
        return 0.0

    values = pd.to_numeric(df["field1"], errors="coerce").dropna()

    if values.empty:
        return 0.0

    mean = values.mean()
    std = values.std()

    # Simple heuristic: mean + 2*std
    return mean + 2 * std


if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: python optimize_thresholds.py <csv_file>")
        sys.exit(1)

    csv_file = sys.argv[1]

    df = pd.read_csv(csv_file)

    recommended = suggest_threshold(df)

    print("Recommended RMS Threshold:")
    print(recommended)