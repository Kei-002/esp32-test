// Define the pins
#define MOISTURE_SENSOR_PIN  34  // Use any ADC pin of ESP32
#define RELAY_PIN 23  // Pin connected to the relay

void setup() {
    Serial.begin(9600); // Start serial communication
    pinMode(RELAY_PIN, OUTPUT);
    digitalWrite(RELAY_PIN, LOW); // Ensure pump is off initially
}

void loop() {
    int sensorValue = analogRead(MOISTURE_SENSOR_PIN); // Read the sensor value
    float moisturePercent = map(sensorValue, 4095, 1500, 0, 100); // Convert to percentage (adjust based on calibration)
    
    // Ensure values stay within 0-100%
    moisturePercent = constrain(moisturePercent, 0, 100);
    
    Serial.print("Moisture Level: ");
    Serial.print(moisturePercent);
    Serial.println("%");
    
    // Control the pump based on moisture levels
    if (moisturePercent < 30) {
        digitalWrite(RELAY_PIN, LOW); // Turn pump on
        Serial.println("Pump ON");
    } else if (moisturePercent >= 50) {
        digitalWrite(RELAY_PIN, HIGH); // Turn pump off
        Serial.println("Pump OFF");
    }
    
    delay(1000); // Wait a second before reading again
}