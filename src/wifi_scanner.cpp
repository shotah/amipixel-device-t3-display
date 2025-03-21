#include "wifi_scanner.h"

namespace WiFiScanner
{
  ScanState currentScanState = SCAN_IDLE;
  String scanResultsJson = "[]";

  void startScan()
  {
    if (currentScanState == SCAN_IDLE)
    {
      Serial.println("[WiFiScanner] Starting asynchronous WiFi scan...");
      WiFi.scanNetworks(true, false); // Start non-blocking scan
      currentScanState = SCAN_STARTING;
    }
  }

  String processScanResults()
  {
    int numberOfNetworks = WiFi.scanComplete();
    JsonDocument jsonDoc;
    JsonArray ssids = jsonDoc.to<JsonArray>();

    if (numberOfNetworks == 0)
    {
      Serial.println("[WiFiScanner] No networks found.");
    }
    else
    {
      Serial.print("[WiFiScanner] Found ");
      Serial.print(numberOfNetworks);
      Serial.println(" networks:");
      for (int i = 0; i < numberOfNetworks; ++i)
      {
        ssids.add(WiFi.SSID(i));
        Serial.print("[WiFiScanner] ");
        Serial.print(i + 1);
        Serial.print(": ");
        Serial.println(WiFi.SSID(i));
      }
    }
    String jsonString;
    serializeJson(jsonDoc, jsonString);
    WiFi.scanDelete();
    return jsonString;
  }

  void handleScan()
  {
    if (currentScanState == SCAN_STARTING)
    {
      currentScanState = SCAN_RUNNING;
    }
    if (currentScanState == SCAN_RUNNING && WiFi.scanComplete() != -1)
    {
      Serial.println("[WiFiScanner] Asynchronous WiFi scan complete.");
      scanResultsJson = processScanResults();
      currentScanState = SCAN_FINISHED;
    }
  }

  String getScanResultsJson()
  {
    if (currentScanState == SCAN_FINISHED)
    {
      currentScanState = SCAN_IDLE;
    }
    return scanResultsJson;
  }

} // namespace WiFiScanner