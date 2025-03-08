#include <Arduino.h>
#include <WiFi.h>
#include <esp_wifi.h>
#include "constants.h" // Include constants.h for MessageIDs and WiFiConfig
#include "lvgl.h"      // Include lvgl.h for lv_msg_send

using namespace Constants;

// --- Function Prototypes ---
static void WiFiEvent(WiFiEvent_t event); // Static, internal event handler

void setupWiFi()
{
    // Set WiFi mode
    WiFi.mode(WIFI_STA);

    // Register WiFi event handler (using static function)
    WiFi.onEvent(WiFiEvent);

    Serial.println("WiFi Module Setup Done. Connecting will be attempted elsewhere.");
    // No WiFi.begin() here in setupWiFi()
}

void connectWiFi()
{ // Implementation for connectWiFi()
    if (WiFi.status() != WL_CONNECTED)
    {
        if (String(WiFiConfig::SSID) != "Your WiFi SSID" || String(WiFiConfig::PASSWORD) != "Your WiFi PASSWORD")
        {
            Serial.println("Connecting to WiFi...");
            WiFi.begin(WiFiConfig::SSID, WiFiConfig::PASSWORD);
        }
        else
        {
            Serial.println("[Error] WiFi SSID and PASSWORD not configured in constants.h!");
        }
    }
    else
    {
        Serial.println("WiFi already connected.");
    }
}

void disconnectWiFi()
{
    Serial.println("Disconnecting WiFi...");
    WiFi.disconnect();
    WiFi.removeEvent(WiFiEvent); // Remove event handler
    WiFi.mode(WIFI_OFF);         // Turn off WiFi module
    Serial.println("WiFi Disconnected.");
}

static void WiFiEvent(WiFiEvent_t event)
{
    Serial.printf("[WiFi-event] event: %d\n", event);

    switch (event)
    {
    case ARDUINO_EVENT_WIFI_READY:
        Serial.println("WiFi interface ready");
        break;
    case ARDUINO_EVENT_WIFI_SCAN_DONE:
        Serial.println("Completed scan for access points");
        break;
    case ARDUINO_EVENT_WIFI_STA_START:
        Serial.println("WiFi client started");
        break;
    case ARDUINO_EVENT_WIFI_STA_STOP:
        Serial.println("WiFi clients stopped");
        break;
    case ARDUINO_EVENT_WIFI_STA_CONNECTED:
        Serial.println("Connected to access point");
        lv_msg_send(static_cast<uint32_t>(MessageIDs::WiFiMessagesEnum::WIFI_CONNECTED_MSG), NULL); // Send CONNECTED message
        break;
    case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
        Serial.println("Disconnected from WiFi access point");
        lv_msg_send(static_cast<uint32_t>(MessageIDs::WiFiMessagesEnum::WIFI_DISCONNECTED_MSG), NULL); // Send DISCONNECTED message
        break;
    case ARDUINO_EVENT_WIFI_STA_AUTHMODE_CHANGE:
        Serial.println("Authentication mode of access point has changed");
        break;
    case ARDUINO_EVENT_WIFI_STA_GOT_IP:
        Serial.print("Obtained IP address: ");
        Serial.println(WiFi.localIP());
        lv_msg_send(static_cast<uint32_t>(MessageIDs::WiFiMessagesEnum::WIFI_GOT_IP_MSG), NULL); // Send GOT_IP message
        break;
    case ARDUINO_EVENT_WIFI_STA_LOST_IP:
        Serial.println("Lost IP address and IP address is reset to 0");
        lv_msg_send(static_cast<uint32_t>(MessageIDs::WiFiMessagesEnum::WIFI_LOST_IP_MSG), NULL); // Send LOST_IP message
        break;
    default:
        break;
    }
}