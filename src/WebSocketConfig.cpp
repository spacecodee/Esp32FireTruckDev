#include "WebSocketConfig.h"

#include "PinDefinitions.h"

WebSocketConfig* WebSocketConfig::instance = nullptr;

WebSocketConfig::WebSocketConfig() : webSocket(WEBSOCKET_PORT) {
    instance = this;  // Store instance pointer
}

void WebSocketConfig::begin() {
    webSocket.begin();
    webSocket.onEvent(webSocketEvent);
}

void WebSocketConfig::loop() { webSocket.loop(); }

void WebSocketConfig::sendData(const JsonDocument& doc) {
    String jsonString;
    serializeJson(doc, jsonString);
    webSocket.broadcastTXT(jsonString);
}

void WebSocketConfig::webSocketEvent(uint8_t num, WStype_t type, uint8_t* payload, size_t length) {
    if (instance) {  // Check if instance exists
        switch (type) {
            case WStype_DISCONNECTED:
                Serial.printf("[%u] Disconnected!\n", num);
                break;
            case WStype_CONNECTED:
                Serial.printf("[%u] Connected!\n", num);
                break;
            case WStype_TEXT: {
                JsonDocument doc;
                deserializeJson(doc, payload);
                instance->handleCommand(doc);  // Call through instance
                break;
            }
                // ... other cases ...
        }
    }
}

void WebSocketConfig::handleCommand(const JsonDocument& doc) {
    if (doc["command"].is<const char*>()) {
        receiveEspControlData(doc);
    }
}

void WebSocketConfig::receiveEspControlData(const JsonDocument& doc) {
    const char* command = doc["command"];

    if (strcmp(command, "led") == 0) {
        const char* led = doc["led"];
        bool state = doc["state"];

        if (strcmp(led, "red") == 0) {
            digitalWrite(LED_RED, state ? HIGH : LOW);
        } else if (strcmp(led, "green") == 0) {
            digitalWrite(LED_GREEN, state ? HIGH : LOW);
        }

        // Send back updated status
        JsonDocument response;
        response["type"] = "status";
        response["led_red"] = digitalRead(LED_RED);
        response["led_green"] = digitalRead(LED_GREEN);
        sendData(response);
    }
}