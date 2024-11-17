#ifndef WIFI_CONFIG_H
#define WIFI_CONFIG_H

#include <WiFiManager.h>

class WifiConfig {
   private:
    const char* apName;
    const char* apPassword;
    bool isConnected;

   public:
    WifiConfig(const char* name = "ESP32_Setup", const char* password = "password");
    bool begin();
    bool isWifiConnected();
    String getLocalIP();
};

#endif