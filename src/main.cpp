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
#define ENA 25                 // Enable Motor A
#define ENB 26                 // Enable Motor B
#define FIRST_RIGHT_WHEEL 27   // Motor A control 1
#define SECOND_RIGHT_WHEEL 14  // Motor A control 2
#define FIRST_LEFT_WHEEL 12    // Motor B control 1
#define SECOND_LEFT_WHEEL 33   // Motor B control 2

#define MIN_ANGLE 30
#define MAX_ANGLE 130
#define INITIAL_POSITION 0
#define SERVO_DELAY 15
// Motor speed
#define MOTOR_SPEED 255

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

        // Red ON, Green OFF
        digitalWrite(LED_RED, HIGH);
        digitalWrite(LED_GREEN, LOW);
        delay(1000);

        // Move servo
        Serial.println("Moving servo");
        moveServo();
        delay(1000);

        // Activate pump
        Serial.println("Activating pump");
        setPumpSpeed(255);  // Turn pump ON
        delay(1000);
        setPumpSpeed(0);  // Turn pump OFF

        // Red OFF, Green ON
        digitalWrite(LED_RED, LOW);
        digitalWrite(LED_GREEN, HIGH);
        delay(1000);
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
    pinMode(FIRST_RIGHT_WHEEL, OUTPUT);
    pinMode(SECOND_RIGHT_WHEEL, OUTPUT);
    pinMode(FIRST_LEFT_WHEEL, OUTPUT);
    pinMode(SECOND_LEFT_WHEEL, OUTPUT);
    pinMode(ENB, OUTPUT);

    // Initial state - motors stopped
    stopMotors();
}

void moveForward() {
    analogWrite(ENA, MOTOR_SPEED);  // Set speed
    analogWrite(ENB, MOTOR_SPEED);  // Set speed

    digitalWrite(FIRST_RIGHT_WHEEL, LOW);
    digitalWrite(SECOND_RIGHT_WHEEL, HIGH);

    digitalWrite(FIRST_LEFT_WHEEL, LOW);
    digitalWrite(SECOND_LEFT_WHEEL, HIGH);
}

void turnLeft() {
    analogWrite(ENA, MOTOR_SPEED);  // Set speed
    analogWrite(ENB, MOTOR_SPEED);  // Set speed

    digitalWrite(FIRST_RIGHT_WHEEL, LOW);
    digitalWrite(SECOND_RIGHT_WHEEL, HIGH);

    digitalWrite(FIRST_LEFT_WHEEL, HIGH);
    digitalWrite(SECOND_LEFT_WHEEL, LOW);
}

void turnRight() {
    analogWrite(ENA, MOTOR_SPEED);  // Set speed
    analogWrite(ENB, MOTOR_SPEED);  // Set speed

    digitalWrite(FIRST_RIGHT_WHEEL, HIGH);
    digitalWrite(SECOND_RIGHT_WHEEL, LOW);

    digitalWrite(FIRST_LEFT_WHEEL, LOW);
    digitalWrite(SECOND_LEFT_WHEEL, HIGH);
}

void moveBackward() {
    analogWrite(ENA, MOTOR_SPEED);  // Set speed
    analogWrite(ENB, MOTOR_SPEED);  // Set speed

    digitalWrite(FIRST_RIGHT_WHEEL, HIGH);
    digitalWrite(SECOND_RIGHT_WHEEL, LOW);

    digitalWrite(FIRST_LEFT_WHEEL, HIGH);
    digitalWrite(SECOND_LEFT_WHEEL, LOW);
}

void stopMotors() {
    digitalWrite(FIRST_RIGHT_WHEEL, LOW);
    digitalWrite(SECOND_RIGHT_WHEEL, LOW);
    digitalWrite(FIRST_LEFT_WHEEL, LOW);
    digitalWrite(SECOND_LEFT_WHEEL, LOW);

    analogWrite(ENA, 0);  // Stop speed
    analogWrite(ENB, 0);  // Stop speed
}