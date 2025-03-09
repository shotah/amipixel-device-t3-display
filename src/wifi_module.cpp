#include "wifi_module.h"
#include "constants.h"
#include <WiFi.h>
#include <esp_event.h>   // Include the header file for WiFi events
#include <esp_wifi.h>    // Include the header file for WiFi event definitions
#include <WebServer.h>   // ESP32 WebServer library
#include "Preferences.h" // For persistent storage - Corrected include
#include <PageBuilder.h> // PageBuilder library
#include <SPIFFS.h>      // For SPIFFS filesystem access
#include <DNSServer.h>   // For Captive Portal DNS

using namespace Constants;

namespace WiFiModule
{
    bool wifi_setup_done = false;
    bool wifi_connected = false;

    Preferences preferences; // For storing WiFi credentials persistently
    WebServer server(80);    // WebServer instance on port 80
    PageBuilder pageBuilder; // PageBuilder instance
    DNSServer dnsServer;     // DNS Server for captive portal

    void setupWiFi()
    {
        Serial.println("WiFi Module Setup Started.");

        // Initialize Preferences for storing WiFi credentials
        if (!preferences.begin("wifi-config", false))
        { // "wifi-config" is the namespace
            Serial.println("[Error] Preferences initialization failed!");
        }

        String savedSSID = preferences.getString("ssid", ""); // "" is the default value if not found
        String savedPassword = preferences.getString("password", "");

        if (savedSSID.length() > 0)
        {
            // We have saved credentials, try to connect in station mode
            Serial.print("Saved WiFi credentials found. Attempting to connect to: ");
            Serial.println(savedSSID);
            connectToWiFi(savedSSID.c_str(), savedPassword.c_str()); // Try connecting with saved credentials
        }
        else
        {
            // No saved credentials, start in Access Point (AP) mode for configuration
            startWiFiAP();
        }

        WiFi.onEvent([](WiFiEvent_t event, WiFiEventInfo_t info)
                     {
            Serial.printf("[WiFi-event] event: %d\n", event);
            switch (event) {
                case WIFI_EVENT_AP_START:
                    //WiFi access point started
                    Serial.println("WiFi AP started");
                    break;
                case WIFI_EVENT_AP_STACONNECTED:
                    //Client connected to soft-AP
                    Serial.println("Client connected to AP");
                    break;
                case WIFI_EVENT_AP_STADISCONNECTED:
                    //Client disconnected from soft-AP
                    Serial.println("Client disconnected from AP");
                    break;
                case WIFI_EVENT_STA_START:
                    Serial.println("WiFi client started");
                    break;
                case WIFI_EVENT_STA_CONNECTED:
                    Serial.println("WiFi connected");
                    wifi_connected = true;
                    break;
                case WIFI_EVENT_STA_DISCONNECTED:
                    Serial.println("WiFi disconnected");
                    wifi_connected = false;
                    break;
                case WIFI_EVENT_STA_GOT_IP:
                    Serial.print("WiFi assigned IP address: ");
                    Serial.println(WiFi.localIP());
                    break;
                default: break;
            } });
        WiFi.disconnect(); // Disconnect if already connected (in case of restart)
        wifi_setup_done = true;
        Serial.println("WiFi Module Setup Done. Connecting will be attempted elsewhere.");
    }

    void startWiFiAP()
    {
        Serial.println("Starting WiFi Access Point (AP) for configuration...");
        WiFi.mode(WIFI_AP);
        WiFi.softAP(WIFI_CONFIG_AP_SSID); // Define WIFI_CONFIG_AP_SSID in constants.h

        IPAddress apIP = WiFi.softAPIP();
        Serial.print("AP IP address: ");
        Serial.println(apIP);

        // Set up DNS server for captive portal
        dnsServer.start(53, "*", apIP); // Port 53, wildcard domain, AP IP

        // Initialize PageBuilder to serve files from SPIFFS
        pageBuilder.begin(SPIFFS, "/");

        // --- Web Server Routes ---

        // Route for the configuration page (wifi_config.htm)
        server.on("/", HTTP_GET, []()
                  {
                      Serial.println("Serving WiFi config page...");
                      pageBuilder.serveStatic("/wifi_config.htm", server); // Serve from SPIFFS
                  });

        // Route to handle form submission (POST request to /config)
        server.on("/config", HTTP_POST, []()
                  {
            Serial.println("WiFi config form submitted...");
            String ssid = server.arg("ssid");
            String password = server.arg("password");

            Serial.print("SSID received: "); Serial.println(ssid);
            Serial.print("Password received: "); Serial.println(password);

            if (ssid.length() > 0) {
                // Save credentials to preferences
                preferences.putString("ssid", ssid);
                preferences.putString("password", password);
                Serial.println("WiFi credentials saved to preferences.");

                server.sendHeader("Location", "/", true); // Redirect back to config page
                server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
                server.sendHeader("Pragma", "no-cache");
                server.sendHeader("Expires", "-1");
                server.send(302, "text/plain", "Redirecting to configuration page..."); // Redirect with success message

                delay(100); // Small delay to send response

                connectToWiFi(ssid.c_str(), password.c_str()); // Attempt to connect to WiFi

            } else {
                // Handle case where SSID is empty (error)
                Serial.println("[Error] SSID cannot be empty.");
                String errorResponse = "Error: SSID cannot be empty.";
                server.sendHeader("Location", "/?error=" + errorResponse, true); // Redirect back to config page with error
                server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
                server.sendHeader("Pragma", "no-cache");
                server.sendHeader("Expires", "-1");
                server.send(302, "text/plain", "Redirecting to configuration page with error..."); // Redirect with error message
            } });

        // Route for handling 404 errors (Page Not Found) - using PageBuilder's 404 handler
        server.onNotFound([]()
                          { pageBuilder.serveNotFound(server); });

        // Start the web server
        server.begin();
        Serial.println("Web server started in AP mode.");
    }

    void connectToWiFi(const char *ssid, const char *password)
    {
        WiFi.mode(WIFI_STA); // Station mode (connect to existing WiFi)
        WiFi.begin(ssid, password);

        Serial.print("Connecting to WiFi SSID: ");
        Serial.println(ssid);

        int retries = 0;
        while (WiFi.status() != WL_CONNECTED && retries < WIFI_MAX_CONNECT_RETRIES)
        {
            delay(1000);
            Serial.print(".");
            retries++;
        }

        if (WiFi.status() == WL_CONNECTED)
        {
            Serial.println("\nWiFi Connected!");
            Serial.print("IP address: ");
            Serial.println(WiFi.localIP());
            wifi_connected = true;
            // Stop AP mode and DNS server if connected in station mode
            WiFi.softAPdisconnect(true); // Disconnect AP interface
            dnsServer.stop();
            Serial.println("WiFi AP and DNS server stopped.");
        }
        else
        {
            Serial.println("\n[Error] WiFi connection failed after multiple retries.");
            startWiFiAP(); // If connection fails, fallback to AP mode
        }
    }

    bool isWifiConnected()
    {
        return wifi_connected;
    }
} // namespace WiFiModule