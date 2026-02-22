"""
Project: sensa-noise
Module: ThingSpeak Data Analysis

Responsibilities:
- Download channel data from ThingSpeak
- Convert JSON to pandas DataFrame
- Plot cry detection events
"""

import requests
import pandas as pd
import matplotlib.pyplot as plt
import sys


def download_thingspeak_data(channel_id: str, read_api_key: str, results: int = 100):
    url = (
        f"https://api.thingspeak.com/channels/{channel_id}/feeds.json"
        f"?api_key={read_api_key}&results={results}"
    )

    response = requests.get(url, timeout=10)
    response.raise_for_status()

    return response.json()


def json_to_dataframe(json_data):
    feeds = json_data.get("feeds", [])
    df = pd.DataFrame(feeds)

    if df.empty:
        print("No data returned from ThingSpeak.")
        return df

    df["created_at"] = pd.to_datetime(df["created_at"])
    df["field1"] = pd.to_numeric(df["field1"], errors="coerce")

    return df


def plot_events(df: pd.DataFrame):
    if df.empty:
        return

    plt.figure(figsize=(10, 5))
    plt.plot(df["created_at"], df["field1"], marker="o")
    plt.title("Cry Detection Events (ThingSpeak)")
    plt.xlabel("Time")
    plt.ylabel("Event Flag")
    plt.grid(True)
    plt.tight_layout()
    plt.show()


if __name__ == "__main__":
    if len(sys.argv) < 3:
        print("Usage: python analyze_thingspeak_data.py <CHANNEL_ID> <READ_API_KEY>")
        sys.exit(1)

    channel_id = sys.argv[1]
    read_api_key = sys.argv[2]

    json_data = download_thingspeak_data(channel_id, read_api_key)
    df = json_to_dataframe(json_data)

    print(df.tail())

    plot_events(df)