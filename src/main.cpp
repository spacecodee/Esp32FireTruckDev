#include <Arduino.h>
#include <ESP32Servo.h>

#include "WifiConfig.h"

#define LED_RED 18
#define LED_GREEN 19

#define LED_PIN 2
#define SERVO_PIN 13  // GPIO13 is a good choice for servo

// Pump configuration
#define PUMP_PIN 22          // GPIO22 for PWM control
#define PUMP_FREQUENCY 5000  // 5KHz PWM frequency
#define PUMP_CHANNEL 0       // PWM channel 0
#define PUMP_RESOLUTION 8    // 8-bit resolution (0-255)

// L298N Motor Driver pins
#define ENA 25  // Enable Motor A
#define ENB 26  // Enable Motor B
#define IN1 27  // Motor A control 1
#define IN2 14  // Motor A control 2
#define IN3 12  // Motor B control 1
#define IN4 33  // Motor B control 2

#define MIN_ANGLE 30
#define MAX_ANGLE 130
#define INITIAL_POSITION 0
#define SERVO_DELAY 15
// Motor speed
#define MOTOR_SPEED 255

// Add to pin definitions
#define FLAME_SENSOR_1 34  // ADC1_6
#define FLAME_SENSOR_2 35  // ADC1_7
#define FLAME_SENSOR_3 32  // ADC1_4

// ADC configuration
#define ADC_RESOLUTION 1023  // 12-bit ADC
#define MAPPED_MIN 1
#define MAPPED_MAX 100

WifiConfig wifi;
Servo myServo;

void setupMotors();
void moveForward();
void moveBackward();
void turnLeft();
void turnRight();
void stopMotors();
void moveServo();
void setupPump();
void controlPump(bool state);
void setPumpSpeed(uint8_t speed);
void setupFlameSensors();
int readFlameValue(int sensorPin);
void readAllFlameSensors();

void setup() {
    Serial.begin(115200);

    // LED setup
    pinMode(LED_RED, OUTPUT);
    pinMode(LED_GREEN, OUTPUT);

    // LED Wi-FI setup
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);

    // Servo setup
    ESP32PWM::allocateTimer(0);
    myServo.setPeriodHertz(50);            // Standard 50hz servo
    myServo.attach(SERVO_PIN, 500, 2400);  // Adjust min/max pulse width if needed
    myServo.write(INITIAL_POSITION);       // Set initial position

    // Motor setup
    setupMotors();

    // Pump setup
    setupPump();

    // WiFi setup
    if (wifi.begin()) {
        Serial.println("Connected!");
        Serial.print("IP Address: ");
        Serial.println(wifi.getLocalIP());
        digitalWrite(LED_PIN, HIGH);
    } else {
        Serial.println("Failed to connect");
        digitalWrite(LED_PIN, LOW);
    }
}

void loop() {
    if (!wifi.isWifiConnected()) {
        digitalWrite(LED_PIN, LOW);
    } else {
        digitalWrite(LED_PIN, HIGH);
        delay(500);
    }
}

void moveServo() {
    for (int angle = MIN_ANGLE; angle <= MAX_ANGLE; angle++) {
        myServo.write(angle);
        delay(SERVO_DELAY);
    }
    for (int angle = MAX_ANGLE; angle >= MIN_ANGLE; angle--) {
        myServo.write(angle);
        delay(SERVO_DELAY);
    }
}

// Update pump setup
void setupPump() {
    ledcSetup(PUMP_CHANNEL, PUMP_FREQUENCY, PUMP_RESOLUTION);
    ledcAttachPin(PUMP_PIN, PUMP_CHANNEL);
    ledcWrite(PUMP_CHANNEL, 0);  // Start with pump off
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