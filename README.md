## CAN-Based Automotive Car Black Box

## 📌 Description

A CAN-based automotive black box system designed to monitor and record important vehicle parameters such as speed, gear, RPM, engine temperature, and indicators using multiple ECUs.

## ⚙️ Features

- Monitors vehicle speed and gear position
- Monitors RPM and engine temperature
- Detects left/right indicators
- Communication between ECUs using CAN protocol
- Displays real-time vehicle information
- Stores/records important vehicle data for analysis

## 🏗️ System Architecture

The system consists of three ECUs:

- **ECU1:** Reads speed and gear information.
- **ECU2:** Reads RPM, engine temperature, and indicator status.
- **ECU3:** Receives CAN messages and displays vehicle information.

## 🛠️ Technologies Used

- **Language:** Embedded C
- **Microcontroller:** PIC18F4580
- **Communication:** CAN
- **IDE:** MPLAB X IDE
- **Compiler:** XC8
- **CAN Transceiver:** MCP2551

## 📡 CAN Message IDs

| Parameter | CAN ID |
|---|---|
| Speed | `0x10` |
| Gear | `0x20` |
| RPM | `0x30` |
| Engine Temperature | `0x40` |
| Indicator | `0x50` |

## 🚀 Applications

- Automotive monitoring
- Vehicle data logging
- Accident/black-box analysis
- Real-time vehicle parameter monitoring
