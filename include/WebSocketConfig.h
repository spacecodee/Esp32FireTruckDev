#ifndef WEBSOCKET_CONFIG_H
#define WEBSOCKET_CONFIG_H

#include <ArduinoJson.h>
#include <WebSocketsServer.h>

class WebSocketConfig {
   private:
    WebSocketsServer webSocket;
    void handleCommand(const JsonDocument& doc);
    static WebSocketConfig* instance;  // Add static instance pointer

   public:
    WebSocketConfig();
    void begin();
    void loop();
    void sendData(const JsonDocument& doc);
    static void webSocketEvent(uint8_t num, WStype_t type, uint8_t* payload, size_t length);
};

#endif