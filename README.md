# 🌱 Smart Plant Monitoring System

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Platform: ESP32](https://img.shields.io/badge/Platform-ESP32-blue.svg)](https://www.espressif.com/en/products/socs/esp32)
[![Arduino Compatible](https://img.shields.io/badge/Arduino-Compatible-green.svg)](https://www.arduino.cc/)

> An intelligent ESP32-based system for automated plant monitoring and care, ensuring optimal growth conditions through precise control of moisture, lighting, temperature, and ventilation.

---

## 📌 Overview

The Smart Plant Monitoring System is a comprehensive solution for automated plant care. It continuously monitors environmental conditions and automatically adjusts various parameters to maintain optimal growing conditions for your plants.

## ✨ Features

- 💧 **Moisture Monitoring & Control**
  - Automatic watering when soil moisture drops below 30%
  - Intelligent pump control system
  
- 💡 **Light Management**
  - Smart grow lights control based on ambient conditions
  - Automated day/night cycle management
  
- 🌡️ **Temperature Control**
  - Automated ventilation system
  - Servo-controlled window management
  - Temperature-triggered fan control
  
- 📱 **Smart Connectivity**
  - Built-in WiFi connectivity
  - Web-based monitoring interface
  - Real-time sensor data access

## 🛠️ Hardware Requirements

| Component | Purpose |
|-----------|---------|
| ESP32 Development Board | Main controller |
| DHT11 | Temperature sensor |
| Capacitive Soil Moisture Sensor | Moisture monitoring |
| LDR | Light sensing |
| Relay Module(s) | Power control |
| Servo Motor | Window control |
| DC Fan | Ventilation |
| Water Pump | Irrigation |
| Grow Lights | Supplemental lighting |

## 📍 Pin Configuration

```cpp
const int MOISTURE_SENSOR_PIN = GPIO34;  // Soil moisture sensor
const int PUMP_RELAY_PIN     = GPIO23;   // Water pump control
const int LDR_SENSOR_PIN     = GPIO35;   // Light sensor (changed from GPIO34)
const int GROWLIGHT_PIN      = GPIO22;   // Grow light control (changed from GPIO23)
const int DHT11_PIN         = GPIO21;    // Temperature sensor
const int SERVO_PIN         = GPIO19;    // Window control
const int FAN_PIN          = GPIO18;     // Ventilation fan
```

## 📚 Required Libraries

| Library | Purpose | Installation | Image |
|---------|---------|--------------|---------|
| [DHT sensor library](https://github.com/adafruit/DHT-sensor-library) | Temperature sensor support | Search "DHT sensor library" | ![Image](https://github.com/user-attachments/assets/3bb4724d-0aab-4e07-8d71-8ce16252c588) |
| [ESP32Servo](https://github.com/madhephaestus/ESP32Servo) | Servo motor control | Search "ESP32Servo" | ![Image](https://github.com/user-attachments/assets/5cfff47a-886c-4f9a-a4a5-0355bb08ab34) |
| [ESPAsyncWebServer](https://github.com/me-no-dev/ESPAsyncWebServer) | Web server functionality | Arduino Library Manager | ![Image](https://github.com/user-attachments/assets/05dc1336-a1f5-4dec-9794-0c5b12104a7d) |
| [AsyncTCP](https://github.com/me-no-dev/AsyncTCP) | TCP support | Included with ESPAsyncWebServer | ![Image](https://github.com/user-attachments/assets/ec9d5e42-5280-403a-8e7b-c94dd48df12d) |

| WiFi | Network connectivity | Built-in with ESP32 | |
| SPIFFS | File system support | Built-in with ESP32 | |

## 🚀 Setup Instructions

1. **Library Installation**
   - Open Arduino IDE
   - Install all required libraries through Library Manager
   - Follow [this tutorial](https://www.youtube.com/watch?v=9i1nDUoDRcI) for SPIFFS setup

2. **WiFi Configuration**
   ```cpp
   const char* ssid = "YOUR_WIFI_SSID";
   const char* password = "YOUR_WIFI_PASSWORD";
   ```

3. **Pin Assignment**
   ```cpp
   #define DHT11_PIN  21  // Temperature sensor
   #define SERVO_PIN  19  // Window control
   #define FAN_PIN    18  // Ventilation fan
   ```

4. **Threshold Configuration**
   ```cpp
   const int MOISTURE_LOW    = 30;   // Pump activation threshold (%)
   const int MOISTURE_HIGH   = 50;   // Pump deactivation threshold (%)
   const int LIGHT_THRESHOLD = 50;   // Grow light activation threshold (%)
   const int TEMP_THRESHOLD  = 30;   // Ventilation activation threshold (°C)
   ```

## ⚙️ Operation

The system operates automatically based on the following logic:

| Condition | Action |
|-----------|--------|
| Moisture < 30% | Activate water pump |
| Light < 50% | Enable grow lights |
| Temperature > 30°C | Open window and activate fan |

## 🌐 Web Interface

- Access sensor data at `http://[ESP32_IP_ADDRESS]/data`
- Real-time monitoring dashboard
- JSON endpoint for integration

Example JSON response:
```json
{
  "moisture": 45,
  "temperature": 25.6,
  "light": 78,
  "pump_status": "OFF",
  "lights_status": "ON",
  "fan_status": "OFF"
}
```

## ⚡ Calibration

Adjust these values based on your specific sensors:

```cpp
// Light sensor calibration
#define DARK_VALUE    4000  // Sensor reading in dark conditions
#define BRIGHT_VALUE  500   // Sensor reading in bright conditions

// Moisture sensor calibration
#define DRY_VALUE    4095   // Sensor reading in dry soil
#define WET_VALUE    2500   // Sensor reading in wet soil
```

## 📚 Resources

- [ESP32 Servo Motor Guide](https://randomnerdtutorials.com/esp32-servo-motor-web-server-arduino-ide/)
- [DHT11 Integration Tutorial](https://esp32io.com/tutorials/esp32-dht11)
- [SPIFFS Web Server Setup](https://randomnerdtutorials.com/esp32-web-server-spiffs-spi-flash-file-system/)

## 📝 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

---

<p align="center">Made with 💚 for plant enthusiasts</p>
