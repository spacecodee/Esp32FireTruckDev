#ifndef WEBSOCKET_CONFIG_H
#define WEBSOCKET_CONFIG_H

#include <ArduinoJson.h>
#include <ESP32Servo.h>
#include <WebSocketsServer.h>

class WebSocketConfig {
   private:
    WebSocketsServer webSocket;
    static WebSocketConfig* instance;
    void handleCommand(const JsonDocument& doc);

   public:
    WebSocketConfig();
    void begin();
    void loop();
    void sendData(const JsonDocument& doc);
    static void webSocketEvent(uint8_t num, WStype_t type, uint8_t* payload, size_t length);

    // Reference to servo
    Servo* servo;
    void setServo(Servo* s) { servo = s; }
    void handleControlCommands(const JsonDocument& doc);
};

#endif