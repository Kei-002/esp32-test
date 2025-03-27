#include <DHT.h>
#include <ESP32Servo.h>
// for wireless capabilities
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
// Define the pins
// MAKE SURE TO CHANGE THIS PINS AS THEY ARE ALL THE SAME
#define MOISTURE_SENSOR_PIN  34  // Use any ADC pin of ESP32
#define PUMP_RELAY_PIN 23  // Pin connected to the relay

#define LDR_PIN 34  // ADC1 pin for LDR
#define GROWLIGHT_RELAY_PIN 23 // Relay control pin

// place a pin here
#define DHT11_PIN = _
#define SERVO_PIN = _
#define FAN_PIN = _

// Define what pins the module uses
DHT dht11(DHT11_PIN, DHT11);
Servo servoMotor;

#define DARK_VALUE 4000  // Adjust based on testing (dark environment)
#define BRIGHT_VALUE 500  // Adjust based on testing (bright environment)

// WIFI Setup
// Replace with your network credentials
const char* ssid = "REPLACE_WITH_YOUR_SSID";
const char* password = "REPLACE_WITH_YOUR_PASSWORD";

AsyncWebServer server(80);


// Variables for readings
float moisturePercent = 0.0;
int lightPercent = 0;
float dht11SensorValue = 0;

void setupWifi() {
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting...");
  }
  Serial.println(WiFi.localIP());

  // Start SPIFFS
  if (!SPIFFS.begin()) {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  // Serve static files
  server.serveStatic("/", SPIFFS, "/").setDefaultFile("index.html");

  // Sensor data endpoint
  server.on("/data", HTTP_GET, [](AsyncWebServerRequest *request){
    String json = "{\"temp\": " + String(dht11SensorValue) + 
                  ", \"moisture\": " + String(moisturePercent) + 
                  ", \"light\": " + String(lightPercent) + "}";
    request->send(200, "application/json", json);
  });

  server.begin();
}

void controlPumpByMoisture(float moisturePercent) {
    Serial.print("Moisture Level: ");
    Serial.print(moisturePercent);
    Serial.println("%");

    if (moisturePercent < 30) {
        digitalWrite(PUMP_RELAY_PIN, LOW); // Turn pump on
        Serial.println("Pump ON");
    } else if (moisturePercent >= 50) {
        digitalWrite(PUMP_RELAY_PIN, HIGH); // Turn pump off
        Serial.println("Pump OFF");
    }
}

// Manage the ldr and growlights here
void controlGrowlightsWithLDR(int lightPercent) {
    Serial.print("Light Intensity: ");
    Serial.print(lightPercent);
    Serial.println("%");

    // Control relay based on light intensity
    if (lightPercent < 50) {  // Example threshold
        digitalWrite(GROWLIGHT_RELAY_PIN, LOW); // Turn on relay in darkness
    } else {
        digitalWrite(GROWLIGHT_RELAY_PIN, HIGH);  // Turn off relay in brightness
    }
}

// Manage fan/servo and dht11 here
void controlFanServoByTemperature(float temp) {
    // Freely change the values that best fits your needs ;)
    // hot hot hot means turn on fan and window(servo)
    if (temp >= 30) {
      // Open window by setting servo angle to 180deg
      
      servoMotor.write(160);
      delay(20);

      // Turn on fan here
      digitalWrite(FAN_PIN, HIGH); 
    }
    else {
      // Close window by setting servo to 0deg
      servoMotor.write(0);
      delay(20);

      // turn off fan here
      digitalWrite(FAN_PIN, LOW);
    }
}

void setup() {
    Serial.begin(9600); // Start serial communication
    pinMode(PUMP_RELAY_PIN, OUTPUT);
    digitalWrite(PUMP_RELAY_PIN, LOW); // Ensure pump is off initially

    pinMode(GROWLIGHT_RELAY_PIN, OUTPUT);
    digitalWrite(GROWLIGHT_RELAY_PIN, LOW); // Ensure relay is off initially

    dht11.begin(); // initialize the DHT11 sensor
    servoMotor.attach(SERVO_PIN);  // initialize the servo  

    pinMode(FAN_PIN, OUTPUT);
    digitalWrite(FAN_PIN, LOW);  // Fan off initially

    setupWifi();
}

void loop() {
    int moistureSensorValue = analogRead(MOISTURE_SENSOR_PIN); // Read the sensor value
    moisturePercent = map(moistureSensorValue, 4095, 1500, 0, 100); // Convert to percentage (adjust based on calibration)
    int ldrValue = analogRead(LDR_PIN); // Read LDR value
    lightPercent = map(ldrValue, DARK_VALUE, BRIGHT_VALUE, 0, 100);
    dht11SensorValue = dht11.readTemperature();

    // Ensure values stay within 0-100%
    moisturePercent = constrain(moisturePercent, 0, 100);
    lightPercent = constrain(lightPercent, 0, 100);

    // Control the pump based on moisture levels
    controlPumpByMoisture(moisturePercent);
    
    // Control the growlights based on the light outside
    controlGrowlightsWithLDR(lightPercent);

    // Control fan based on temperature
    controlFanServoByTemperature(dht11SensorValue);

    // Wait a second before reading again
    delay(1000); 
}

// Manage the Pump and Moisture Sensor here


// REMEMBER TO REMOVE SOME OF THE COMMENTS :>
// MAKE SURE TO INSTALL THE MODULES (refer below)
// Resources for the modules needED for the servo and dht11
// https://randomnerdtutorials.com/esp32-servo-motor-web-server-arduino-ide/      SERVO TUTS
// https://esp32io.com/tutorials/esp32-dht11                                      DHT11 TUTS

// For wifi
// https://randomnerdtutorials.com/esp32-web-server-spiffs-spi-flash-file-system/#:~:text=With%20SPIFFS%2C%20you%20can%20write,documented%20on%20its%20GitHub%20page.

