#include <Arduino.h>

#include "WifiConfig.h"

#define LED_PIN 2

WifiConfig wifi;

void setup() {
    Serial.begin(115200);
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);

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
    }
    delay(100);
}