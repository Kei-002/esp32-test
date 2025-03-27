#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include <DHT.h>
#include <ArduinoJson.h>
#include <ESP32Servo.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

// WiFi credentials
const char* ssid = "YourWiFiSSID";
const char* password = "YourWiFiPassword";

// Firebase credentials
#define API_KEY "YOUR-FIREBASE-API-KEY"
#define DATABASE_URL "YOUR-FIREBASE-DATABASE-URL"

// Pin definitions
#define DHT_PIN 4
#define DHT_TYPE DHT22
#define SOIL_MOISTURE_PIN 34
#define LDR_PIN 35
#define PUMP_PIN 16
#define GROW_LIGHTS_PIN 17
#define FAN_PIN 18
#define SERVO_PIN 19

// Initialize components
DHT dht(DHT_PIN, DHT_TYPE);
Servo windowServo;
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

    Serial.begin(115200);
    
    // Initialize SPIFFS
    if(!SPIFFS.begin(true)) {
        Serial.println("SPIFFS Mount Failed");
        return;
    }

    // Initialize pins
    pinMode(PUMP_PIN, OUTPUT);
    pinMode(GROW_LIGHTS_PIN, OUTPUT);
    pinMode(FAN_PIN, OUTPUT);
    
    // Initialize servo
    windowServo.attach(SERVO_PIN);
    windowServo.write(0); // Close window by default
    
    // Connect to WiFi
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }
    Serial.println("Connected to WiFi");
    Serial.println("IP address: " + WiFi.localIP().toString());

    // Initialize DHT sensor
    dht.begin();

    // Route for root directory and static files
    server.on("/", HTTP_GET, handleRoot);
    server.on("/bootstrap.min.css", HTTP_GET, []() {
        File file = SPIFFS.open("/bootstrap.min.css", "r");
        server.streamFile(file, "text/css");
        file.close();
    });
    server.on("/styles.css", HTTP_GET, []() {
        File file = SPIFFS.open("/styles.css", "r");
        server.streamFile(file, "text/css");
        file.close();
    });
    server.on("/jquery-3.7.1.min.js", HTTP_GET, []() {
        File file = SPIFFS.open("/jquery-3.7.1.min.js", "r");
        server.streamFile(file, "application/javascript");
        file.close();
    });
    server.on("/bootstrap.min.js", HTTP_GET, []() {
        File file = SPIFFS.open("/bootstrap.min.js", "r");
        server.streamFile(file, "application/javascript");
        file.close();
    });
    server.on("/script.js", HTTP_GET, []() {
        File file = SPIFFS.open("/script.js", "r");
        server.streamFile(file, "application/javascript");
        file.close();
    });

    // Routes for sensor data and control
    server.on("/data", HTTP_GET, handleData);
    server.on("/window", HTTP_POST, handleWindow);
    server.on("/fan", HTTP_POST, handleFan);
    server.on("/wateringlog", HTTP_GET, handleWateringLog);

    server.begin();
}

void loop() {
    server.handleClient();
    checkAndControlDevices();
    delay(100);
}

void checkAndControlDevices() {
    float moisture = getMoisture();
    float temperature = dht.readTemperature();
    
    // Automatic window control based on temperature
    if (temperature > 30) {
        windowServo.write(90); // Open window
    } else if (temperature < 25) {
        windowServo.write(0); // Close window
    }
    
    // Fan control based on temperature
    if (temperature > 28) {
        digitalWrite(FAN_PIN, HIGH);
    } else {
        digitalWrite(FAN_PIN, LOW);
    }
    
    // Water pump control and logging
    if (moisture < 30) { // If soil is too dry
        digitalWrite(PUMP_PIN, HIGH);
        // Log watering event
        if (millis() - lastWateringTime > 300000) { // 5 minutes between logs
            logWatering(moisture);
            lastWateringTime = millis();
        }
    } else {
        digitalWrite(PUMP_PIN, LOW);
    }
}

void logWatering(float moisture) {
    time_t now;
    time(&now);
    wateringLogs[currentLogIndex].timestamp = now;
    wateringLogs[currentLogIndex].moisture = moisture;
    
    currentLogIndex = (currentLogIndex + 1) % MAX_WATERING_LOGS;
    if (totalLogs < MAX_WATERING_LOGS) totalLogs++;
}

void handleRoot() {
    File file = SPIFFS.open("/index.html", "r");
    server.streamFile(file, "text/html");
    file.close();
}

void handleData() {
    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();
    
    int moistureRaw = analogRead(SOIL_MOISTURE_PIN);
    float moisture = map(moistureRaw, 4095, 0, 0, 100);
    
    int lightRaw = analogRead(LDR_PIN);
    float light = map(lightRaw, 4095, 0, 0, 100);
    
    StaticJsonDocument<256> doc;
    doc["temperature"] = isnan(temperature) ? 0 : temperature;
    doc["humidity"] = isnan(humidity) ? 0 : humidity;
    doc["moisture"] = moisture;
    doc["light"] = light;
    doc["pump"] = digitalRead(PUMP_PIN);
    doc["growlights"] = digitalRead(GROW_LIGHTS_PIN);
    doc["fan"] = digitalRead(FAN_PIN);
    doc["window"] = windowServo.read() > 45;

    String response;
    serializeJson(doc, response);
    server.send(200, "application/json", response);
}

void handleWindow() {
    if (server.hasArg("open")) {
        bool open = server.arg("open") == "true";
        windowServo.write(open ? 90 : 0);
        server.send(200, "text/plain", "Window position updated");
    } else {
        server.send(400, "text/plain", "Missing parameter");
    }
}

void handleFan() {
    if (server.hasArg("on")) {
        bool on = server.arg("on") == "true";
        digitalWrite(FAN_PIN, on ? HIGH : LOW);
        server.send(200, "text/plain", "Fan status updated");
    } else {
        server.send(400, "text/plain", "Missing parameter");
    }
}

void handleWateringLog() {
    StaticJsonDocument<2048> doc;
    JsonArray logs = doc.createNestedArray("logs");
    
    int startIndex = currentLogIndex - totalLogs;
    if (startIndex < 0) startIndex += MAX_WATERING_LOGS;
    
    for (int i = 0; i < totalLogs; i++) {
        int index = (startIndex + i) % MAX_WATERING_LOGS;
        JsonObject log = logs.createNestedObject();
        log["timestamp"] = wateringLogs[index].timestamp;
        log["moisture"] = wateringLogs[index].moisture;
    }
    
    String response;
    serializeJson(doc, response);
    server.send(200, "application/json", response);
}

float getMoisture() {
    int moistureRaw = analogRead(SOIL_MOISTURE_PIN);
    return map(moistureRaw, 4095, 0, 0, 100);
}
