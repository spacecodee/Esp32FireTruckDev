#include "WebSocketConfig.h"

#include "PinDefinitions.h"

WebSocketConfig* WebSocketConfig::instance = nullptr;

WebSocketConfig::WebSocketConfig() : webSocket(WEBSOCKET_PORT) {
    instance = this;  // Store instance pointer
}

void WebSocketConfig::begin() {
    Serial.println("Starting WebSocket server...");

    // Wait for WiFi connection
    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 10) {
        Serial.print(".");
        delay(1000);
        attempts++;
    }

    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("WiFi connection failed!");
        return;
    }

    // Verify network config
    IPAddress ip = WiFi.localIP();
    if (ip[0] == 0) {
        Serial.println("Invalid IP address!");
        return;
    }

    // Print detailed network info
    Serial.println("\nNetwork Configuration:");
    Serial.printf("IP: %s\n", WiFi.localIP().toString().c_str());
    Serial.printf("Subnet: %s\n", WiFi.subnetMask().toString().c_str());
    Serial.printf("Gateway: %s\n", WiFi.gatewayIP().toString().c_str());
    Serial.printf("DNS: %s\n", WiFi.dnsIP().toString().c_str());
    Serial.printf("Channel: %d\n", WiFi.channel());
    Serial.printf("RSSI: %d dBm\n", WiFi.RSSI());

    Serial.println("\nStarting WebSocket on:");
    Serial.printf("ws://%s:%d\n", WiFi.localIP().toString().c_str(), WEBSOCKET_PORT);

    webSocket.begin();
    webSocket.enableHeartbeat(15000, 3000, 2);
    webSocket.onEvent(webSocketEvent);

    Serial.println("WebSocket server ready!");
}

void WebSocketConfig::loop() {
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("WiFi connection lost!");
        return;
    }
    webSocket.loop();
}

void WebSocketConfig::sendData(const JsonDocument& doc) {
    String jsonString;
    serializeJson(doc, jsonString);
    webSocket.broadcastTXT(jsonString);
}

void WebSocketConfig::webSocketEvent(uint8_t num, WStype_t type, uint8_t* payload, size_t length) {
    if (!instance) return;

    JsonDocument doc;

    switch (type) {
        case WStype_t::WStype_DISCONNECTED:
            Serial.printf("[WS] Client #%u disconnected\n", num);
            break;

        case WStype_t::WStype_CONNECTED:
            Serial.printf("[WS] Client #%u connected from %s\n", num,
                          instance->webSocket.remoteIP(num).toString().c_str());
            instance->sendEspConnectionData();
            break;

        case WStype_t::WStype_TEXT: {  // Added scope brackets
            Serial.printf("[%u] Received text: %s\n", num, payload);
            DeserializationError error = deserializeJson(doc, payload);
            if (!error) {
                instance->handleCommand(doc);
            } else {
                Serial.println("Failed to parse JSON command");
            }
            break;
        }

        case WStype_t::WStype_ERROR:
            Serial.printf("[WS] Error from client #%u\n", num);
            break;

        default:
            break;
    }
}

void WebSocketConfig::handleCommand(const JsonDocument& doc) {
    if (doc["command"].is<const char*>()) {
        handleControlCommands(doc);
    }
}

void WebSocketConfig::handleControlCommands(const JsonDocument& doc) {
    const char* command = doc["command"];

    if (strcmp(command, "led") == 0) {
        const char* led = doc["led"];
        bool state = doc["state"];

        // Add debug logging
        Serial.printf("LED Command - Type: %s, State: %d\n", led, state);

        if (strcmp(led, "red") == 0) {
            digitalWrite(LED_RED, state ? HIGH : LOW);
        } else if (strcmp(led, "green") == 0) {
            digitalWrite(LED_GREEN, state ? HIGH : LOW);
        }

        // Send LED status
        JsonDocument response;
        response["type"] = "status";
        response["led_red"] = digitalRead(LED_RED);
        response["led_green"] = digitalRead(LED_GREEN);
        sendData(response);
    } else if (strcmp(command, "servo") == 0 && servo != nullptr) {
        Serial.println("Received servo sweep command");

        for (int pos = 0; pos <= 180; pos += 1) {
            servo->write(pos);
            delay(15);

            JsonDocument response;
            response["type"] = "servo_status";
            response["position"] = pos;
            sendData(response);
        }

        for (int pos = 180; pos >= 0; pos -= 1) {
            servo->write(pos);
            delay(15);

            JsonDocument response;
            response["type"] = "servo_status";
            response["position"] = pos;
            sendData(response);
        }
    }
}

void WebSocketConfig::sendEspConnectionData() {
    JsonDocument doc;
    doc["type"] = "connection";
    doc["connected"] = (WiFi.status() == WL_CONNECTED);
    sendData(doc);
}