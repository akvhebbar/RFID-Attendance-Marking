# RFID-Attendance-Marking
An automated, IoT-based attendance system using an ESP32 microcontroller and RFID cards. Features real-time data logging to Google Sheets and instant SMS alerts via the Twilio API.
# 📡 RFID-Based Attendance Marking System

![Project Banner](images/results.jpg) *<- Replace with a picture of your working project or the Google Sheets result*

## 📌 Project Overview
[cite_start]This project is an automated, Internet of Things (IoT)-based attendance system designed to replace manual roll calls[cite: 5]. [cite_start]It utilizes an **ESP32 microcontroller** and an **MFRC522 RFID reader** to identify students uniquely[cite: 8, 9]. [cite_start]Upon a successful scan, the system logs the attendance instantly to a centralized **Google Sheet** and sends an automated **SMS alert** to administrators via the **Twilio API**[cite: 5, 43]. 

## ✨ Key Features
* [cite_start]**Contactless Identification:** Rapid student verification using RFID cards and local UID mapping[cite: 10, 12].
* [cite_start]**Real-Time Cloud Logging:** Transmits data via HTTP POST requests to a Google Apps Script, appending records to a Google Sheet[cite: 18, 19].
* [cite_start]**Instant Notifications:** Integrates with Twilio's REST API to send SMS alerts verifying the student's presence[cite: 21, 22].
* [cite_start]**Tamper-Resistant Timestamping:** Fetches accurate Indian Standard Time (IST) directly from an NTP server (pool.ntp.org), ensuring reliable records[cite: 13, 16].
* [cite_start]**Security Validation:** The system actively rejects unauthorized/unregistered RFID cards[cite: 11, 242].

## 🛠️ Hardware Requirements
* [cite_start]**Microcontroller:** ESP32 Development Board [cite: 89]
* [cite_start]**Sensor:** MFRC522 RFID Reader [cite: 91]
* [cite_start]**Tokens:** RFID Cards/Tags (13.56 MHz) [cite: 91]
* [cite_start]**Status Indicator:** On-board ESP32 LED [cite: 103]

## 🔌 Circuit Diagram & Pin Configuration
*![Block Diagram](images/block_diagram.jpg)* *<- Replace with your block diagram*

[cite_start]The MFRC522 reader communicates with the ESP32 via the SPI bus[cite: 97]. 

| ESP32 Pin | MFRC522 Pin | Function |
| :--- | :--- | :--- |
| GPIO 18 | SCK | [cite_start]SPI Clock [cite: 98] |
| GPIO 23 | MOSI | [cite_start]Master Out Slave In [cite: 99] |
| GPIO 19 | MISO | [cite_start]Master In Slave Out [cite: 100] |
| GPIO 5 | SDA / SS | [cite_start]Slave Select [cite: 101] |
| GPIO 21 | RST | [cite_start]Reset [cite: 102] |
| GPIO 2 | - | [cite_start]Status LED [cite: 104] |

## 💻 Software & Libraries Used
[cite_start]This project was programmed using the **Arduino IDE**[cite: 109]. Ensure the following libraries are installed:
* [cite_start]`SPI.h` (Hardware SPI communication) [cite: 111]
* [cite_start]`MFRC522.h` (RFID sensor library) [cite: 112]
* [cite_start]`WiFi.h` (ESP32 network connectivity) [cite: 113]
* [cite_start]`HTTPClient.h` & `base64.h` (REST API communication and Twilio authentication) [cite: 114, 115]
* [cite_start]`time.h` (NTP time synchronization) [cite: 116]

## 🚀 Setup & Installation
1. **Hardware Setup:** Wire the ESP32 and RFID reader according to the pinout table above.
2. **Google Sheets:** Create a new Google Sheet, set up a Google Apps Script to accept POST requests, and deploy it as a Web App to get your Webhook URL.
3. **Twilio Setup:** Create a Twilio account to get your `Account SID`, `Auth Token`, and a virtual phone number.
4. **Code Configuration:** Open `src/main.ino` and update the following variables:
   * [cite_start]WiFi `ssid` and `password` [cite: 125, 126]
   * [cite_start]Twilio credentials (`accountSid`, `authToken`, `twilioNumber`, `adminNumber`) [cite: 128-131]
   * [cite_start]Google Sheets `sheetsUrl` [cite: 133]
   * [cite_start]Update the `students[]` array with your specific RFID UIDs [cite: 140-144]
5. **Flash:** Upload the code to your ESP32.

## 📊 Results & Latency
[cite_start]During testing, the end-to-end data flow—from the moment a card is tapped to the row appearing in Google Sheets and the SMS being delivered—takes approximately **1 to 3 seconds**[cite: 241, 243].
