#ifndef WIFI_SCANNER_H
#define WIFI_SCANNER_H

#include <Arduino.h>
#include <WiFi.h>
#include <ArduinoJson.h>

namespace WiFiScanner
{
    enum ScanState
    {
        SCAN_IDLE,
        SCAN_STARTING,
        SCAN_RUNNING,
        SCAN_FINISHED
    };

    extern ScanState currentScanState;

    void startScan();
    void handleScan();
    String getScanResultsJson();

} // namespace WiFiScanner

#endif // WIFI_SCANNER_H