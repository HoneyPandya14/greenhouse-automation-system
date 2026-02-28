# Greenhouse Automation System (Embedded C++ Project)

## Overview

This project implements a real-time greenhouse monitoring and control prototype using Arduino and C++.

The system integrates multiple analogue sensors, converts raw electrical signals into physical measurements, and applies rule-based decision logic to automate environmental indicators.

Built entirely from scratch, including circuit design, sensor modelling, threshold calibration, and embedded software architecture.

---

## System Capabilities

The system monitors:

- Ambient light intensity (LDR)
- Temperature (NTC thermistor using Beta model)
- Soil moisture level

And responds by:

- Indicating night/day/bright conditions
- Activating heater indicator when temperature < 5°C
- Signalling irrigation requirement when soil is dry

---

## Architecture

### Input Layer
- ADC-based analogue readings
- Voltage-to-resistance transformations
- Sensor-specific modelling equations

### Processing Layer
- Lux estimation from LDR
- Temperature calculation via Beta equation
- Threshold-based classification
- Independent modular functions

### Output Layer
- Multi-LED environmental status indicators
- Continuous real-time update loop

---

## Engineering Principles Demonstrated

- Embedded C++ development
- Real-time data acquisition
- Signal conditioning using voltage dividers
- Nonlinear thermistor modelling
- Sensor calibration and threshold tuning
- Modular system design
- Hardware-software integration
- Debugging via serial diagnostics

---

## Design Philosophy

The system is structured into independent functional modules:

- `timeOfDay()`
- `temperature()`
- `waterInThePlants()`

Each module handles:
1. Data acquisition
2. Physical transformation
3. Decision logic
4. Output control

The `loop()` function orchestrates these modules continuously, enabling real-time system behaviour.

---

## Calibration

Key thresholds:

- Soil dryness threshold: 300 (ADC units)
- Heater activation: Temperature < 5°C
- Lux classification thresholds adjustable for environment tuning

Thresholds are intentionally defined as constants to allow environment-specific calibration without restructuring core logic.

---

## Why This Project Matters

This project demonstrates the ability to:

- Design an end-to-end technical system
- Convert analogue sensor data into meaningful physical metrics
- Apply mathematical modelling in embedded systems
- Build modular, maintainable C++ code
- Integrate multiple subsystems into a cohesive architecture
- Develop and validate a working prototype

---

## Future Extensions

- LCD-based real-time display
- Wireless monitoring (IoT integration)
- Humidity and CO₂ sensing
- Closed-loop actuator control instead of LED indicators
