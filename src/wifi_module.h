#ifndef WIFI_MODULE_H // Updated to WIFI_MODULE_H
#define WIFI_MODULE_H // Updated to WIFI_MODULE_H

#include <Arduino.h>
#include <WiFi.h>

// --- Function Prototypes ---
void setupWiFi();
void connectWiFi();                // Add this line: prototype for connectWiFi()
void WiFiEvent(WiFiEvent_t event); // Keep this - WiFi event handler
void disconnectWiFi();             // Add this line - for disconnecting WiFi

#endif /* WIFI_H */