#include <Arduino.h>
#include <ESP32Servo.h>

#include "WifiConfig.h"

#define LED_PIN 2
#define SERVO_PIN 13  // GPIO13 is a good choice for servo
#define MIN_ANGLE 30
#define MAX_ANGLE 130
#define INITIAL_POSITION 0
#define SERVO_DELAY 15

WifiConfig wifi;
Servo myServo;

void setup() {
    Serial.begin(115200);

    // LED setup
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);

    // Servo setup
    ESP32PWM::allocateTimer(0);
    myServo.setPeriodHertz(50);            // Standard 50hz servo
    myServo.attach(SERVO_PIN, 500, 2400);  // Adjust min/max pulse width if needed
    myServo.write(INITIAL_POSITION);       // Set initial position

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

        // Smooth servo movement
        for (int angle = MIN_ANGLE; angle <= MAX_ANGLE; angle++) {
            myServo.write(angle);
            delay(SERVO_DELAY);
        }
        for (int angle = MAX_ANGLE; angle >= MIN_ANGLE; angle--) {
            myServo.write(angle);
            delay(SERVO_DELAY);
        }
    }
}