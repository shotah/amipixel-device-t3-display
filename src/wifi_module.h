#ifndef WIFI_MODULE_H
#define WIFI_MODULE_H

#include <Arduino.h> // NOLINT(readability-magic-numbers)
#include <WiFi.h>

namespace WiFiModule
{ // Add namespace declaration

    // --- Function Prototypes ---
    void setupWiFi();
    bool connectToWiFi(const char *ssid, const char *password);
    void disconnectWiFi();

} // namespace WiFiModule

#endif /* WIFI_MODULE_H */