#include <Arduino.h>
#include <DNSServer.h>
#include <WebServer.h>
#include <WiFiManager.h>

#define LED_PIN 2  // Built-in LED on most ESP32 Dev boards

void setup() {
    Serial.begin(115200);

    // Setup LED pin
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);  // Ensure LED starts off

    // Initialize WiFiManager
    WiFiManager wifiManager;

    // Set custom AP name
    if (wifiManager.autoConnect("ESP32_Setup", "password")) {
        Serial.println("Connected!");
        Serial.print("IP Address: ");
        Serial.println(WiFi.localIP());

        // Turn ON LED when connected
        digitalWrite(LED_PIN, HIGH);
    } else {
        Serial.println("Failed to connect");
        digitalWrite(LED_PIN, LOW);
    }
}

void loop() {
    // Keep LED ON while WiFi is connected
    if (WiFi.status() != WL_CONNECTED) {
        digitalWrite(LED_PIN, LOW);
    } else {
        digitalWrite(LED_PIN, HIGH);
    }
    delay(100);  // Small delay to prevent busy-waiting
}