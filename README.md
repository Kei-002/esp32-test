# Smart Plant Monitoring System

An ESP32-based automated plant monitoring and control system that manages moisture levels, lighting, temperature, and ventilation for optimal plant growth conditions.

## Features

- **Moisture Monitoring & Control**: Automatically waters plants when soil moisture drops below 30%
- **Light Management**: Controls grow lights based on ambient light conditions
- **Temperature Control**: Manages ventilation through fan and servo-controlled window
- **WiFi Connectivity**: Provides web interface for monitoring sensor data
- **Real-time Monitoring**: Continuous monitoring of all environmental parameters

## Hardware Requirements

- ESP32 Development Board
- DHT11 Temperature Sensor
- Capacitive Soil Moisture Sensor
- LDR (Light Dependent Resistor)
- Relay Module(s)
- Servo Motor
- DC Fan
- Water Pump
- Grow Lights

## Pin Configuration

```cpp
Moisture Sensor: GPIO34
Pump Relay: GPIO23
LDR Sensor: GPIO34
Grow Light Relay: GPIO23
DHT11: [Need to specify pin]
Servo Motor: [Need to specify pin]
Fan: [Need to specify pin]
```

**Note**: Some pins are currently sharing the same GPIO. Please modify the pin assignments in the code to use different GPIO pins for each component.

## Required Libraries

- `DHT sensor library by Adafruit` (search for "DHT sensor library")
![alt text](image-2.png)

- `ESP32Servo by Kevin Harrington` (search for "ESP32Servo")
![alt text](image-3.png)

- `ESPAsyncWebServer by lacamera` (download from Arduino Library Manager)
![alt text](image.png)

- `AsyncTCP by lacamera` (included in the ESPAsyncWebServer installation)
![alt text](image-1.png)

- `WiFi` (built-in with ESP32 board package)
- `SPIFFS` (built-in with ESP32 board package)  (follow this tutorial for installation) [YouTube](https://www.youtube.com/watch?v=9i1nDUoDRcI)


## Setup Instructions

1. Install required libraries in Arduino IDE
2. Configure WiFi credentials:
   ```cpp
   const char* ssid = "YOUR_WIFI_SSID";
   const char* password = "YOUR_WIFI_PASSWORD";
   ```
3. Set appropriate pins for DHT11, Servo, and Fan in the code:
   ```cpp
   #define DHT11_PIN = [your_pin]
   #define SERVO_PIN = [your_pin]
   #define FAN_PIN = [your_pin]
   ```
4. Adjust threshold values if needed:
   - Moisture threshold: 30% (pump on) to 50% (pump off)
   - Light threshold: 50% for grow lights control
   - Temperature threshold: 30°C for ventilation control

## Operation

The system operates automatically with the following logic:

- Waters plants when moisture level falls below 30%
- Activates grow lights in low light conditions (below 50% ambient light)
- Opens window and activates fan when temperature exceeds 30°C
- Provides sensor data through web interface at `[ESP32_IP_ADDRESS]/data`

## Web Interface

The system hosts a web server that provides:
- Real-time sensor data in JSON format
- Web-based monitoring interface
- Endpoint: `/data` returns current sensor readings in JSON format

## Calibration

You may need to adjust these values based on your setup:
```cpp
#define DARK_VALUE 4000  // Adjust for your light sensor in dark conditions
#define BRIGHT_VALUE 500 // Adjust for your light sensor in bright conditions
```

## Resources

For detailed setup of individual components:
- [ESP32 Servo Motor Tutorial](https://randomnerdtutorials.com/esp32-servo-motor-web-server-arduino-ide/)
- [ESP32 DHT11 Tutorial](https://esp32io.com/tutorials/esp32-dht11)
- [ESP32 SPIFFS Web Server Tutorial](https://randomnerdtutorials.com/esp32-web-server-spiffs-spi-flash-file-system/)
