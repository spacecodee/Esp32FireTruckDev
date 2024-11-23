#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESP32Servo.h>

#include "PinDefinitions.h"
#include "WebSocketConfig.h"
#include "WifiConfig.h"

WifiConfig wifi;
WebSocketConfig webSocket;
Servo myServo;

void setupMotors();
void moveForward();
void moveBackward();
void turnLeft();
void turnRight();
void stopMotors();
void setupPump();
void controlPump(bool state);
void setPumpSpeed(uint8_t speed);
void setupFlameSensors();
int readFlameValue(int sensorPin);
void readAllFlameSensors();
void sendEspConnectionData();

void setup() {
    Serial.begin(115200);
    delay(2000);
    Serial.println("\n\n=== ESP32 Starting ===");

    ESP32PWM::allocateTimer(0);
    myServo.setPeriodHertz(50);  // standard 50 hz servo
    myServo.attach(MY_SERVO_PIN, 1000, 2000);
    myServo.write(0);

    pinMode(LED_PIN, OUTPUT);
    pinMode(LED_RED, OUTPUT);
    pinMode(LED_GREEN, OUTPUT);
    digitalWrite(LED_PIN, LOW);
    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_GREEN, LOW);
    Serial.println("LEDs initialized");

    // Pump Setup
    setupPump();

    if (wifi.begin()) {
        digitalWrite(LED_PIN, HIGH);
        webSocket.begin();
        webSocket.setServo(&myServo);
        Serial.println("WiFi Connected!");
        Serial.println(wifi.getLocalIP());
    }
}

void loop() {
    if (!wifi.isWifiConnected()) {
        digitalWrite(LED_PIN, LOW);
        stopMotors();                // Safety: stop motors if WiFi disconnects
        ledcWrite(PUMP_CHANNEL, 0);  // Safety: stop pump if WiFi disconnects
    } else {
        digitalWrite(LED_PIN, HIGH);
        webSocket.loop();  // Handle WebSocket events

        // Only send connection data every 500ms
        static unsigned long lastUpdate = 0;
        if (millis() - lastUpdate > 500) {
            sendEspConnectionData();
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

// Add pump speed control
void setPumpSpeed(uint8_t speed) {
    // speed: 0-255 (0=off, 255=full speed)
    ledcWrite(PUMP_CHANNEL, speed);
}

void controlPump(bool state) {
    // state: true = ON, false = OFF
    digitalWrite(PUMP_PIN, state ? HIGH : LOW);
}

void setupMotors() {
    // Configure motor control pins
    pinMode(ENA, OUTPUT);
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);
    pinMode(ENB, OUTPUT);

    // Initial state - motors stopped
    stopMotors();
}

void moveForward() {
    analogWrite(ENA, MOTOR_SPEED);  // Set speed
    analogWrite(ENB, MOTOR_SPEED);  // Set speed

    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);

    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
}

void turnLeft() {
    analogWrite(ENA, MOTOR_SPEED);  // Set speed
    analogWrite(ENB, MOTOR_SPEED);  // Set speed

    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);

    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
}

void turnRight() {
    analogWrite(ENA, MOTOR_SPEED);  // Set speed
    analogWrite(ENB, MOTOR_SPEED);  // Set speed

    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);

    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
}

void moveBackward() {
    analogWrite(ENA, MOTOR_SPEED);  // Set speed
    analogWrite(ENB, MOTOR_SPEED);  // Set speed

    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);

    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
}

void stopMotors() {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);

    analogWrite(ENA, 0);  // Stop speed
    analogWrite(ENB, 0);  // Stop speed
}

void setupFlameSensors() {
    // Set pins as inputs
    pinMode(FLAME_SENSOR_1, INPUT);
    pinMode(FLAME_SENSOR_2, INPUT);
    pinMode(FLAME_SENSOR_3, INPUT);
}

int readFlameValue(int sensorPin) {
    int rawValue = analogRead(sensorPin);
    // Map from 0-1023 to 1-100 range
    return map(rawValue, 0, ADC_RESOLUTION, MAPPED_MIN, MAPPED_MAX);
}

void readAllFlameSensors() {
    int flame1 = readFlameValue(FLAME_SENSOR_1);
    int flame2 = readFlameValue(FLAME_SENSOR_2);
    int flame3 = readFlameValue(FLAME_SENSOR_3);

    Serial.printf("Flame Sensors: %d%%, %d%%, %d%%\n", flame1, flame2, flame3);
}

// Function to send sensor data
void sendEspConnectionData() {
    JsonDocument doc;
    doc["type"] = "connection";
    doc["connected"] = wifi.isWifiConnected();

    webSocket.sendData(doc);
}