#define LDR_PIN 34  // ADC1 pin for LDR
#define RELAY_PIN 23 // Relay control pin

#define DARK_VALUE 4000  // Adjust based on testing (dark environment)
#define BRIGHT_VALUE 500  // Adjust based on testing (bright environment)

void setup() {
    Serial.begin(9600);  // Start Serial Monitor
    pinMode(RELAY_PIN, OUTPUT);
    digitalWrite(RELAY_PIN, LOW); // Ensure relay is off initially
}

void loop() {
    int sensorValue = analogRead(LDR_PIN); // Read LDR value
    int lightPercent = map(sensorValue, DARK_VALUE, BRIGHT_VALUE, 0, 100);
    lightPercent = constrain(lightPercent, 0, 100); // Keep within 0-100%

    // Print to Serial Monitor
    Serial.print("Raw LDR Value: ");
    Serial.println(sensorValue);
    Serial.print("Light Intensity: ");
    Serial.print(lightPercent);
    Serial.println("%");

    // Control relay based on light intensity
    if (lightPercent < 50) {  // Example threshold
        digitalWrite(RELAY_PIN, LOW); // Turn on relay in darkness
    } else {
        digitalWrite(RELAY_PIN, HIGH);  // Turn off relay in brightness
    }
    
    delay(1000); // Read every second
}