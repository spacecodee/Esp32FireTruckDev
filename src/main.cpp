#include <Arduino.h>
#include <ESP32Servo.h>

#include "WifiConfig.h"

#define LED_RED 16
#define LED_PIN 2
#define SERVO_PIN 13  // GPIO13 is a good choice for servo
#define PUMP_PIN 4

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
void setupPump();
void setPumpSpeed(int speed);

void setup() {
    Serial.begin(115200);

    // LED setup
    pinMode(LED_RED, OUTPUT);
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
    digitalWrite(LED_RED, HIGH);
    delay(1000);
    digitalWrite(LED_RED, LOW);
    delay(1000);
}

// Add pump control functions
void setupPump() {
    pinMode(PUMP_PIN, OUTPUT);
    digitalWrite(PUMP_PIN, LOW);  // Ensure pump is off initially
}

void setPumpSpeed(int speed) {
    // speed: 0-255 (0=off, 255=full speed)
    analogWrite(PUMP_PIN, speed);
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