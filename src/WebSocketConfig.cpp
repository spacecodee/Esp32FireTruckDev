#include "WebSocketConfig.h"

#include <WiFiManager.h>

#include "PinDefinitions.h"

WebSocketConfig* WebSocketConfig::instance = nullptr;

WebSocketConfig::WebSocketConfig(WifiConfig& wifiConfig) : webSocket(WEBSOCKET_PORT), wifi(wifiConfig) {
    instance = this;
}

void WebSocketConfig::begin() {
    Serial.println("Starting WebSocket server...");

    // Wait for Wi-Fi connection
    int attempts = 0;
    while (WiFiClass::status() != WL_CONNECTED && attempts < 10) {
        Serial.print(".");
        delay(1000);
        attempts++;
    }

    if (WiFiClass::status() != WL_CONNECTED) {
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
    if (WiFiClass::status() != WL_CONNECTED) {
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

void WebSocketConfig::sendLedStatus() {
    JsonDocument response;
    response["type"] = "status";
    response["led_red"] = digitalRead(LED_RED);
    response["led_green"] = digitalRead(LED_GREEN);
    sendData(response);
}

void WebSocketConfig::webSocketEvent(const uint8_t num, const WStype_t type, uint8_t* payload, size_t length) {
    if (!instance) return;

    switch (type) {
        case WStype_DISCONNECTED:
            Serial.printf("[WS] Client #%u disconnected\n", num);
            break;

        case WStype_CONNECTED:
            Serial.printf("[WS] Client #%u connected from %s\n", num,
                          instance->webSocket.remoteIP(num).toString().c_str());
            instance->sendEspConnectionData();
            instance->sendLedStatus();  // Send initial LED states
            break;

        case WStype_TEXT: {
            JsonDocument doc;
            // Added scope brackets
            Serial.printf("[%u] Received text: %p\n", num, payload);
            const DeserializationError error = deserializeJson(doc, payload);
            if (!error) {
                instance->handleCommand(doc);
            } else {
                Serial.println("Failed to parse JSON command");
            }
            break;
        }

        case WStype_ERROR:
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

    if (strcmp(command, "motor") == 0) {
        const char* direction = doc["direction"];
        Serial.printf("Motor Command Received - Direction: %s\n", direction);

        if (strcmp(direction, "forward") == 0) {
            moveForward();
        } else if (strcmp(direction, "backward") == 0) {
            moveBackward();
        } else if (strcmp(direction, "forward_left") == 0) {
            turnForwardLeft();
        } else if (strcmp(direction, "forward_right") == 0) {
            turnForwardRight();
        } else if (strcmp(direction, "backward_left") == 0) {
            turnBackwardLeft();
        } else if (strcmp(direction, "backward_right") == 0) {
            turnBackwardRight();
        } else if (strcmp(direction, "stop") == 0) {
            stopMotors();
        }

        JsonDocument response;
        response["type"] = "motor_status";
        response["direction"] = direction;
        response["speed_a"] = ledcRead(MOTOR_A_CHANNEL);
        response["speed_b"] = ledcRead(MOTOR_B_CHANNEL);
        sendData(response);
    }
    // Add this new else-if block
    else if (strcmp(command, "wifi_reset") == 0) {
        Serial.println("Resetting WiFi settings...");

        // Send confirmation before resetting
        JsonDocument response;
        response["type"] = "wifi_reset";
        response["status"] = "resetting";
        sendData(response);

        delay(1000);  // Give time for the response to be sent

        // Reset WiFi settings and restart
        WifiConfig::resetWiFiSettings();
    } else if (strcmp(command, "led") == 0) {
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
    } else if (strcmp(command, "pump") == 0) {
        bool state = doc["state"];
        Serial.printf("Pump Command - State: %d\n", state);

        if (state) {
            ledcWrite(PUMP_CHANNEL, 255);  // Full speed
        } else {
            ledcWrite(PUMP_CHANNEL, 0);  // Stop
        }

        // Send pump status
        JsonDocument response;
        response["type"] = "pump_status";
        response["state"] = state;
        response["speed"] = state ? 255 : 0;
        sendData(response);
    }
}

void WebSocketConfig::sendEspConnectionData() {
    JsonDocument doc;
    doc["type"] = "connection";
    doc["connected"] = WiFiClass::status() == WL_CONNECTED;
    sendData(doc);
}

int WebSocketConfig::readFlameValue(const int sensorPin) {
    const int rawValue = analogRead(sensorPin);
    // Map from 0-4095 to 1-100 range (using ESP32's 12-bit ADC)
    return map(rawValue, 0, ADC_RESOLUTION, MAPPED_MIN, MAPPED_MAX);
}

void WebSocketConfig::sendFlameSensorData() {
    JsonDocument doc;
    doc["type"] = "flame_sensors";
    doc["sensor1"] = readFlameValue(FLAME_SENSOR_1);
    doc["sensor2"] = readFlameValue(FLAME_SENSOR_2);
    doc["sensor3"] = readFlameValue(FLAME_SENSOR_3);
    sendData(doc);
}

void WebSocketConfig::moveForward() {
    // Set speeds
    ledcWrite(MOTOR_A_CHANNEL, MOTOR_SPEED);
    ledcWrite(MOTOR_B_CHANNEL, MOTOR_SPEED);

    // Left Motors
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);

    // Right Motors
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
}

void WebSocketConfig::moveBackward() {
    ledcWrite(MOTOR_A_CHANNEL, MOTOR_SPEED);
    ledcWrite(MOTOR_B_CHANNEL, MOTOR_SPEED);

    // Left Motors
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);

    // Right Motors
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
}

void WebSocketConfig::turnForwardLeft() {
    ledcWrite(MOTOR_A_CHANNEL, MOTOR_SPEED);
    ledcWrite(MOTOR_B_CHANNEL, MOTOR_SPEED);

    // Left Motors
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);

    // Right Motors
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
}

void WebSocketConfig::turnForwardRight() {
    ledcWrite(MOTOR_A_CHANNEL, MOTOR_SPEED);
    ledcWrite(MOTOR_B_CHANNEL, MOTOR_SPEED);

    // Left Motors
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);

    // Right Motors
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
}

void WebSocketConfig::turnBackwardLeft() {
    ledcWrite(MOTOR_A_CHANNEL, MOTOR_SPEED);
    ledcWrite(MOTOR_B_CHANNEL, MOTOR_SPEED);

    // Left Motors - Stop
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);

    // Right Motors - Backward
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
}

void WebSocketConfig::turnBackwardRight() {
    ledcWrite(MOTOR_A_CHANNEL, MOTOR_SPEED);
    ledcWrite(MOTOR_B_CHANNEL, MOTOR_SPEED);

    // Left Motors - Backward
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);

    // Right Motors - Stop
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
}

void WebSocketConfig::stopMotors() {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);

    ledcWrite(MOTOR_A_CHANNEL, 0);  // Stop using PWM channel
    ledcWrite(MOTOR_B_CHANNEL, 0);
}