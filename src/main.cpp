#include <Arduino.h>

#include "PinDefinitions.h"
#include "WebSocketConfig.h"

class WifiConfig;

WifiConfig wifi;
WebSocketConfig webSocket(wifi);  // Pass wifi reference

void setupMotors();
void stopMotors();
void setupPump();
void setupFlameSensors();

void setup() {
    Serial.begin(115200);
    delay(2000);
    Serial.println("\n\n=== ESP32 Starting ===");

    pinMode(LED_PIN, OUTPUT);
    pinMode(LED_RED, OUTPUT);
    pinMode(LED_GREEN, OUTPUT);
    digitalWrite(LED_PIN, LOW);
    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_GREEN, LOW);
    Serial.println("LEDs initialized");

    // Pump Setup
    setupPump();
    setupMotors();

    if (wifi.begin()) {
        digitalWrite(LED_PIN, HIGH);
        webSocket.begin();
        Serial.println("WiFi Connected!");
        Serial.println(WifiConfig::getLocalIP());
    }
}

void loop() {
    if (!WifiConfig::isWifiConnected()) {
        digitalWrite(LED_PIN, LOW);
        stopMotors();                // Safety: stop motors if Wi-Fi disconnects
        ledcWrite(PUMP_CHANNEL, 0);  // Safety: stop pump if Wi-Fi disconnects
    } else {
        digitalWrite(LED_PIN, HIGH);
        webSocket.loop();  // Handle WebSocket events

        // Only send connection data every 500 ms
        static unsigned long lastUpdate = 0;
        if (millis() - lastUpdate > 500) {
            webSocket.sendEspConnectionData();
            webSocket.sendFlameSensorData();
            lastUpdate = millis();
        }
    }
}

void setupPump() {
    ledcSetup(PUMP_CHANNEL, PUMP_FREQUENCY, PUMP_RESOLUTION);
    ledcAttachPin(PUMP_PIN, PUMP_CHANNEL);
    ledcWrite(PUMP_CHANNEL, 0);  // Start with pump off
    Serial.println("Pump initialized");
}

void setupMotors() {
    // Configure PWM for motors
    ledcSetup(MOTOR_A_CHANNEL, MOTOR_FREQUENCY, MOTOR_RESOLUTION);
    ledcSetup(MOTOR_B_CHANNEL, MOTOR_FREQUENCY, MOTOR_RESOLUTION);
    ledcAttachPin(ENA, MOTOR_A_CHANNEL);
    ledcAttachPin(ENB, MOTOR_B_CHANNEL);

    // Configure direction control pins
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);

    stopMotors();
}

void stopMotors() {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);

    ledcWrite(MOTOR_A_CHANNEL, 0);  // Stop using PWM channel
    ledcWrite(MOTOR_B_CHANNEL, 0);
}

void setupFlameSensors() {
    // Set pins as inputs
    pinMode(FLAME_SENSOR_1, INPUT);
    pinMode(FLAME_SENSOR_2, INPUT);
    pinMode(FLAME_SENSOR_3, INPUT);
}