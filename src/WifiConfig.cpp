#include "WifiConfig.h"

WifiConfig::WifiConfig(const char* name, const char* password) {
    apName = name;
    apPassword = password;
    isConnected = false;
}

bool WifiConfig::begin() {
    WiFiManager wifiManager;
    isConnected = wifiManager.autoConnect(apName, apPassword);
    return isConnected;
}

bool WifiConfig::isWifiConnected() { return WiFiClass::status() == WL_CONNECTED; }

String WifiConfig::getLocalIP() { return WiFi.localIP().toString(); }

void WifiConfig::resetWiFiSettings() {
    WiFiManager wifiManager;
    wifiManager.resetSettings();
    ESP.restart();  // Restart ESP32 to apply changes
}