#ifndef WEBSOCKET_CONFIG_H
#define WEBSOCKET_CONFIG_H

#include <ArduinoJson.h>
#include <ESP32Servo.h>
#include <WebSocketsServer.h>

class WebSocketConfig {
    WebSocketsServer webSocket;

    void handleCommand(const JsonDocument& doc);
    Servo* servo{};        // Add servo pointer
    void sendLedStatus();  // Add this method declaration

   public:
    WebSocketConfig();

    void begin();
    void loop();
    void moveServo();
    void sendData(const JsonDocument& doc);
    void sendEspConnectionData();
    void handleControlCommands(const JsonDocument& doc);
    void setServo(Servo* s) { servo = s; }  // Add setter
    static void moveForward();
    static void moveBackward();
    static void turnForwardLeft();
    static void turnForwardRight();
    static void stopMotors();

    static WebSocketConfig* instance;
    static void webSocketEvent(uint8_t num, WStype_t type, uint8_t* payload, size_t length);
};

#endif