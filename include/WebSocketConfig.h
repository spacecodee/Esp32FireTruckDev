#ifndef WEBSOCKET_CONFIG_H
#define WEBSOCKET_CONFIG_H

#include <ArduinoJson.h>
#include <WebSocketsServer.h>
#include <WifiConfig.h>

class WebSocketConfig {
    WebSocketsServer webSocket;
    static WebSocketConfig* instance;
    WifiConfig& wifi;  // Add reference

    static int readFlameValue(int sensorPin);
    void handleCommand(const JsonDocument& doc);
    void sendLedStatus();  // Add this method declaration

   public:
    explicit WebSocketConfig(WifiConfig& wifiConfig);

    void begin();
    void loop();
    void sendData(const JsonDocument& doc);
    void sendEspConnectionData();
    void handleControlCommands(const JsonDocument& doc);
    void sendFlameSensorData();

    static void moveForward();
    static void moveBackward();
    static void turnForwardLeft();
    static void turnForwardRight();
    static void turnBackwardLeft();
    static void turnBackwardRight();
    static void stopMotors();
    static void webSocketEvent(uint8_t num, WStype_t type, uint8_t* payload, size_t length);
};

#endif