#ifndef WIFI_MODULE_H
#define WIFI_MODULE_H

#include <Arduino.h> // NOLINT(readability-magic-numbers)
#include <WiFi.h>

namespace WiFiModule { // Add namespace declaration

// --- Function Prototypes ---
void setupWiFi();
void connectWiFi();
void disconnectWiFi();

} // namespace WiFiModule

#endif /* WIFI_MODULE_H */