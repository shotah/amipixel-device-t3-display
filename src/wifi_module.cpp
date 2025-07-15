#include "wifi_module.h"
#include "Preferences.h"
#include "constants.h"
#include "fs_handler.h"
#include "wifi_scanner.h"
#include <ArduinoJson.h>
#include <DNSServer.h>
#include <SPIFFS.h>
#include <WebServer.h>
#include <WiFi.h>

namespace WiFiModule
{

  bool wifi_setup_done = false;
  bool wifi_connected = false;
  bool sleep_flag = false;
  bool getSleepFlag() { return sleep_flag; }
  void setSleepFlag(bool flag) { sleep_flag = flag; }

  void WiFiEvent(WiFiEvent_t event);
  bool isWifiConnected() { return wifi_connected; }

  Preferences preferences; // For storing WiFi credentials persistently
  WebServer server(80);
  DNSServer dnsServer;

  void startWiFiAP();
  String scanWiFiNetworks();


  // Storage for registered callbacks
  static wifi_event_callback_t wifi_connected_callbacks[5] = {nullptr};  // Support up to 5 callbacks
  static wifi_event_callback_t wifi_disconnected_callbacks[5] = {nullptr};
  static int connected_callback_count = 0;
  static int disconnected_callback_count = 0;

  // Function to call all registered connected callbacks
  static void call_connected_callbacks() {
      Serial.printf("[WiFi] Calling %d connected callbacks...\n", connected_callback_count);
      for (int i = 0; i < connected_callback_count; i++) {
          if (wifi_connected_callbacks[i] != nullptr) {
              Serial.printf("[WiFi] Calling connected callback #%d\n", i + 1);
              wifi_connected_callbacks[i]();
          }
      }
      Serial.println("[WiFi] All connected callbacks completed");
  }

  // Function to call all registered disconnected callbacks
  static void call_disconnected_callbacks() {
      Serial.printf("[WiFi] Calling %d disconnected callbacks...\n", disconnected_callback_count);
      for (int i = 0; i < disconnected_callback_count; i++) {
          if (wifi_disconnected_callbacks[i] != nullptr) {
              Serial.printf("[WiFi] Calling disconnected callback #%d\n", i + 1);
              wifi_disconnected_callbacks[i]();
          }
      }
      Serial.println("[WiFi] All disconnected callbacks completed");
  }



  void setupWiFi()
  {
    Serial.println("WiFi Module Setup Started.");

    WiFi.onEvent(WiFiEvent);

    preferences.begin("wifi-config", false);

    // --- Add this section to clear saved credentials ---
    if (preferences.getString("ssid", "").length() > 0)
    {
      Serial.println("Clearing saved WiFi credentials.");
      preferences.remove("ssid");
      preferences.remove("password");
    }
    // --- End of credential clearing ---

    String savedSSID = preferences.getString("ssid", "");
    String savedPassword = preferences.getString("password", "");
    preferences.end();

    if (savedSSID.length() > 0)
    {
      Serial.print("Saved WiFi credentials found. Attempting to connect to: ");
      Serial.println(savedSSID);
      connectToWiFi(savedSSID.c_str(), savedPassword.c_str());
    }
    else
    {
      Serial.println(
          "No saved WiFi credentials. Starting AP mode for configuration.");
      startWiFiAP();
    }
    
    wifi_setup_done = true;
    Serial.println(
        "WiFi Module Setup Done. Connecting will be attempted elsewhere.");
  }

  void handleScanTriggerRequest()
  {
    Serial.println("[WiFiModule] /scan-trigger request received.");
    WiFiScanner::startScan();
    server.send(200, "text/plain", "Scan triggered");
  }

  void handleGetSSIDsRequest()
  {
    Serial.println("[WiFiModule] /ssids request received.");
    server.send(200, "application/json", WiFiScanner::getScanResultsJson());
  }

  void manageWiFiConnection()
  {
    server.handleClient();
    dnsServer.processNextRequest();
    WiFiScanner::handleScan();
  }

  String urlEncode(const String &str)
  {
    String encodedStr = "";
    for (int i = 0; i < str.length(); i++)
    {
      if (isAlphaNumeric(str.charAt(i)) || str.charAt(i) == '-' ||
          str.charAt(i) == '_' || str.charAt(i) == '.')
      {
        encodedStr += str.charAt(i);
      }
      else if (str.charAt(i) == ' ')
      {
        encodedStr += "%20";
      }
      else
      {
        char buf[4];
        sprintf(buf, "%%%02X", str.charAt(i));
        encodedStr += buf;
      }
    }
    return encodedStr;
  }

  void startWiFiAP()
  {
    Serial.println("Starting WiFi Access Point (AP) for configuration...");
    WiFi.mode(WIFI_AP);
    WiFi.softAP(Constants::WiFiConfig::SSID); // Use SSID from constants.h

    IPAddress apIP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(apIP);

    dnsServer.start(53, "*", apIP);

    // --- Web Server Routes - Using serveStatic directly ---
    server.serveStatic("/", SPIFFS, "/index.htm");
    server.serveStatic("/img/", SPIFFS, "/img/");
    server.serveStatic("/js/", SPIFFS, "/js/");
    server.serveStatic("/css/", SPIFFS, "/css/");
    server.serveStatic("/success.htm", SPIFFS, "/success.htm");
    server.serveStatic("/error.htm", SPIFFS, "/error.htm");
    server.serveStatic("/not_found", SPIFFS, "/not_found.htm");

    // New routes to handle WiFi scan and return SSIDs as JSON
    server.on("/scan-trigger", HTTP_GET, handleScanTriggerRequest);
    server.on("/ssids", HTTP_GET, handleGetSSIDsRequest);
    server.on("/config", HTTP_POST, []()
              {
    String ssid = server.arg("ssid");
    String password = server.arg("password");

    bool connectionSuccess = connectToWiFi(ssid.c_str(), password.c_str());

    if (connectionSuccess) {
      server.sendHeader("Location", "/success.htm", true);

      // Save credentials only on successful connection
      preferences.begin("wifi-config", false);
      preferences.putString("ssid", ssid);
      preferences.putString("password", password);
      preferences.end();
    } else {
      String errorMsg = "Error: WiFi connection failed.";
      String errorUrl = "/error.htm?error=" + urlEncode(errorMsg);
      server.sendHeader("Location", errorUrl, true);
    }
    server.send(302, "text/plain", ""); });

    // Not Found
    // Redirect all requests to /index.htm
    server.onNotFound([]()
                      {
    server.sendHeader("Location", "/", true);
    server.send(302, "text/plain", ""); });

    server.begin();
    Serial.println("Web server started in AP mode.");
  }

  bool connectToWiFi(const char *ssid, const char *password)
  {
    
    Serial.println("Connecting to WiFi...");
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    int retries = 0;
    while (WiFi.status() != WL_CONNECTED &&
           retries < Constants::WiFiConfig::WIFI_MAX_CONNECT_RETRIES)
    {
      delay(500);
      Serial.print(".");
      retries++;
    }

    if (WiFi.status() == WL_CONNECTED)
    {
      Serial.println("");
      Serial.print("WiFi connected! IP address: ");
      Serial.println(WiFi.localIP());
      return true;
    }
    else
    {
      Serial.println("");
      Serial.println("WiFi connection failed.");
      startWiFiAP(); // Re-start AP mode if connection fails
      return false;
    }
  }

  void WiFiEvent(WiFiEvent_t event)
  {
    String savedSSID;
    String savedPassword;

    switch (event)
    {
    case WIFI_EVENT_STA_START:
      Serial.println("[WiFi-event] STA Started");
      break;
    case WIFI_EVENT_STA_DISCONNECTED:
      wifi_connected = false;
      Serial.println("[WiFi-event] STA Disconnected");
      // Call registered callbacks when WiFi disconnects
      call_disconnected_callbacks();
      Serial.println("Attempting to reconnect in Station mode...");
      preferences.begin("wifi-config", false);
      savedSSID = preferences.getString("ssid", "");
      savedPassword = preferences.getString("password", "");
      preferences.end();
      connectToWiFi(savedSSID.c_str(),
                    savedPassword.c_str()); // Reconnect using saved credentials
      break;
    case WIFI_EVENT_STA_CONNECTED:
      Serial.println("[WiFi-event] STA Connected");
      break;
    case ARDUINO_EVENT_WIFI_STA_GOT_IP:
      Serial.print("[WiFi-event] STA Got IP: ");
      Serial.println(WiFi.localIP());
      wifi_connected = true;
      // Call registered callbacks when WiFi connects
      call_connected_callbacks();
      break;
    case ARDUINO_EVENT_WIFI_STA_LOST_IP:
      wifi_connected = false;
      Serial.println("[WiFi-event] STA Lost IP");
      // Call registered callbacks when WiFi disconnects
      call_disconnected_callbacks();
      break;
    case WIFI_EVENT_AP_START:
      Serial.println("[WiFi-event] AP Started");
      break;
    case WIFI_EVENT_AP_STOP:
      Serial.println("[WiFi-event] AP Stopped");
      break;
    case WIFI_EVENT_AP_STACONNECTED:
      Serial.println("[WiFi-event] Client connected to AP");
      break;
    case WIFI_EVENT_AP_STADISCONNECTED:
      Serial.println("[WiFi-event] Client disconnected from AP");
      break;
    default:
      Serial.printf("[WiFi-event] event: %d\n", event);
      break;
    }
  }

  void disconnectWiFi()
  {
    WiFi.disconnect();
    // WiFi.removeEvent(WiFiEvent);
    WiFi.mode(WIFI_OFF);

    Serial.println();
  };



} // namespace WiFiModule

// Callback registration functions (outside namespace for global access)

// Function to register WiFi connected callback
void register_wifi_connected_callback(wifi_event_callback_t callback) {
    if (WiFiModule::connected_callback_count < 5 && callback != nullptr) {
        WiFiModule::wifi_connected_callbacks[WiFiModule::connected_callback_count++] = callback;
        Serial.printf("[WiFi] Registered connected callback #%d\n", WiFiModule::connected_callback_count);
    } else {
        Serial.println("[WiFi] ERROR: Cannot register more connected callbacks or callback is null");
    }
}

// Function to register WiFi disconnected callback  
void register_wifi_disconnected_callback(wifi_event_callback_t callback) {
    if (WiFiModule::disconnected_callback_count < 5 && callback != nullptr) {
        WiFiModule::wifi_disconnected_callbacks[WiFiModule::disconnected_callback_count++] = callback;
        Serial.printf("[WiFi] Registered disconnected callback #%d\n", WiFiModule::disconnected_callback_count);
    } else {
        Serial.println("[WiFi] ERROR: Cannot register more disconnected callbacks or callback is null");
    }
}