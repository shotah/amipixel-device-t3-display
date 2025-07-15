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
    void manageWiFiConnection();
    void disableAPMode(); // New function to prevent AP beacon crashes

} // namespace WiFiModule

// Type definition for WiFi event callbacks
typedef void (*wifi_event_callback_t)(void);

// WiFi event callback registration functions (outside namespace for global access)
void register_wifi_connected_callback(wifi_event_callback_t callback);
void register_wifi_disconnected_callback(wifi_event_callback_t callback);

#endif /* WIFI_MODULE_H */
