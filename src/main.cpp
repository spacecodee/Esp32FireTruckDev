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
void stopMotors();
void moveServo();

void setupPump();
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
    setupMotors();

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
        stopMotors();                // Safety: stop motors if Wi-Fi disconnects
        ledcWrite(PUMP_CHANNEL, 0);  // Safety: stop pump if Wi-Fi disconnects
    } else {
        digitalWrite(LED_PIN, HIGH);
        webSocket.loop();  // Handle WebSocket events

        // Only send connection data every 500 ms
        static unsigned long lastUpdate = 0;
        if (millis() - lastUpdate > 500) {
            sendEspConnectionData();
            lastUpdate = millis();
        }
    }
}

void moveServo() {
    for (int pos = 0; pos <= 180; pos += 1) {
        myServo.write(pos);
        delay(15);
    }

    for (int pos = 180; pos >= 0; pos -= 1) {
        myServo.write(pos);
        delay(15);
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

int readFlameValue(const int sensorPin) {
    const int rawValue = analogRead(sensorPin);
    // Map from 0-1023 to 1-100 range
    return map(rawValue, 0, ADC_RESOLUTION, MAPPED_MIN, MAPPED_MAX);
}

void readAllFlameSensors() {
    const int flame1 = readFlameValue(FLAME_SENSOR_1);
    const int flame2 = readFlameValue(FLAME_SENSOR_2);
    const int flame3 = readFlameValue(FLAME_SENSOR_3);

    Serial.printf("Flame Sensors: %d%%, %d%%, %d%%\n", flame1, flame2, flame3);
}

// Function to send sensor data
void sendEspConnectionData() {
    JsonDocument doc;
    doc["type"] = "connection";
    doc["connected"] = wifi.isWifiConnected();

    webSocket.sendData(doc);
}