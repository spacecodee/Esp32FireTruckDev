#include "WebSocketConfig.h"

#include "PinDefinitions.h"

WebSocketConfig::WebSocketConfig() : webSocket(WEBSOCKET_PORT) {}

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
    switch (type) {
        case WStype_DISCONNECTED:
            Serial.printf("[%u] Disconnected!\n", num);
            break;
        case WStype_CONNECTED:
            Serial.printf("[%u] Connected!\n", num);
            break;
    }
}