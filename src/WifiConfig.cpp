#include "WifiConfig.h"

WifiConfig::WifiConfig(const char* name, const char* password) {
    apName = name;
    apPassword = password;
    isConnected = false;
}

bool WifiConfig::begin() {
    WiFiManager wifiManager;
    wifiManager.resetSettings();
    isConnected = wifiManager.autoConnect(apName, apPassword);
    return isConnected;
}

bool WifiConfig::isWifiConnected() { return WiFi.status() == WL_CONNECTED; }

String WifiConfig::getLocalIP() { return WiFi.localIP().toString(); }