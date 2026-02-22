# ThingSpeak Setup Guide — Sensa-Noise

## 1. Overview

This document describes how to configure ThingSpeak for the Sensa-Noise cry detection system.

ThingSpeak is used to:

- Store cry detection events
- Provide historical telemetry
- Generate datasets for AI analysis
- Support threshold optimization

---

# 2. Create a ThingSpeak Account

1. Go to: https://thingspeak.com
2. Sign up or log in.
3. Navigate to **Channels** → **New Channel**.

---

# 3. Create a New Channel

## Channel Configuration

Channel Name:
Sensa-Noise Cry Detection

Fields:

- Field 1 → Cry Event Flag

Optional:
- Enable "Show Channel Location"
- Enable "Make Public" (only if desired)

Click **Save Channel**.

---

# 4. Obtain API Keys

After creating the channel:

Go to:

Channel → API Keys

You will find:

- Write API Key
- Read API Key
- Channel ID

You will use:

- Write API Key → for ESP32 firmware
- Read API Key → for Python analysis
- Channel ID → for dataset downloads

---

# 5. Configure ESP32 Firmware

Open:
# ThingSpeak Setup Guide — Sensa-Noise

## 1. Overview

This document describes how to configure ThingSpeak for the Sensa-Noise cry detection system.

ThingSpeak is used to:

- Store cry detection events
- Provide historical telemetry
- Generate datasets for AI analysis
- Support threshold optimization

---

# 2. Create a ThingSpeak Account

1. Go to: https://thingspeak.com
2. Sign up or log in.
3. Navigate to **Channels** → **New Channel**.

---

# 3. Create a New Channel

## Channel Configuration

Channel Name:
Sensa-Noise Cry Detection

Fields:

- Field 1 → Cry Event Flag

Optional:
- Enable "Show Channel Location"
- Enable "Make Public" (only if desired)

Click **Save Channel**.

---

# 4. Obtain API Keys

After creating the channel:

Go to:

Channel → API Keys

You will find:

- Write API Key
- Read API Key
- Channel ID

You will use:

- Write API Key → for ESP32 firmware
- Read API Key → for Python analysis
- Channel ID → for dataset downloads

---

# 5. Configure ESP32 Firmware

Open:
firmware/esp32/config.h

Add:

```cpp
#ifndef CONFIG_H
#define CONFIG_H

#define WIFI_SSID "YOUR_WIFI_NAME"
#define WIFI_PASSWORD "YOUR_WIFI_PASSWORD"

#define THINGSPEAK_HOST "http://api.thingspeak.com"
#define THINGSPEAK_API_KEY "YOUR_WRITE_API_KEY"

#endif
```

⚠ SECURITY RULES

Never commit config.h

Ensure config.h is listed in .gitignore

Never expose API keys publicly

6. Data Format Sent by ESP32

When a cry event is detected:

GET /update?api_key=WRITE_API_KEY&field1=1

Meaning:

field1 = 1 → Cry detected

Future expansions may include:

field2 → RMS value

field3 → Duration

field4 → Frequency band energy

7. Verify Data Upload

After uploading firmware:

Open your ThingSpeak channel.

Go to the Private View tab.

Trigger a cry event (or simulate).

Confirm that field1 receives value 1.

If data does not appear:

Check WiFi connection

Check API key

Verify HTTP response code in Serial Monitor

8. Download Data for AI Analysis

Use:

analyze_thingspeak_data.py

Example:

python analyze_thingspeak_data.py CHANNEL_ID READ_API_KEY

This will:

Download JSON data

Convert to pandas DataFrame

Plot cry detection events

9. Rate Limits

ThingSpeak Free Tier Limits:

15 seconds minimum between updates

Limited message count per year

Ensure the firmware does not spam events.

10. Future Improvements

Possible cloud upgrades:

Switch to MQTT

Store full RMS time-series

Integrate with AWS IoT

Add mobile dashboard

Real-time push notifications

11. Security Best Practices

Mandatory:

INMP441 operates at 3.3V only

Never use 5V on sensors

config.h must remain local

Do not upload SSID/password

Do not upload API keys

12. Data Flow Summary

INMP441
→ ESP32 I2S
→ RMS + Duration Logic
→ Event Trigger
→ HTTP GET
→ ThingSpeak Channel
→ Python AI Analysis

ThingSpeak enables persistent cloud storage while maintaining edge autonomy and secure credential handling.