#ifndef WIFI_CONFIG_H
#define WIFI_CONFIG_H

#include <WiFiManager.h>

class WifiConfig {
    const char* apName;
    const char* apPassword;
    bool isConnected;

   public:
    explicit WifiConfig(const char* name = "ESP32_Setup", const char* password = "password");
    bool begin();
    static bool isWifiConnected();
    static String getLocalIP();
    static void resetWiFiSettings();  // Add this new method
};

#endif