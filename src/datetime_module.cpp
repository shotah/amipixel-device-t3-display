#include "datetime_module.h"
#include "constants.h"
#include "root_ca.h"
#include "wifi_module.h" // Include for WiFiMessagesEnum
#include <HTTPClient.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <esp_sntp.h>
#include <lvgl.h> // For lv_msg and message IDs
#include <lwip/apps/sntp.h>

using namespace Constants;

namespace DateTime {
static WiFiUDP ntpUDP;
static NTPClient timeClient(ntpUDP);
static WiFiClientSecure client; // Static client for timezone API
static HTTPClient https;        // Static https for timezone API
static LilyGo_Class *amoled_ptr = nullptr;

// Forward declaration for WiFi callback  
void onWifiConnected();

// --- Static Function Prototypes ---
static void time_available(struct timeval *t); // Local time_available callback

void setupDateTime(LilyGo_Class &amoled_instance) {
  amoled_ptr = &amoled_instance;
  // Note: WiFi connection handling moved to direct callback in wifi_module
  Serial.println("DateTime module initialized");
}

// This function will be called directly by wifi_module when connection is established
void onWifiConnected() {
  Serial.println("DateTime Module received WiFi connection");
  // Start the datetime sync task when WiFi is connected
  static TaskHandle_t datetimeTaskHandle =
      NULL; // Static handle to avoid re-creation
  if (datetimeTaskHandle == NULL) {
    xTaskCreate(
        datetimeSyncTask, "datetimeSync", 10 * 1024, NULL, 11,
        &datetimeTaskHandle); // Slightly higher priority than weather/coin
  }
}

void datetimeSyncTask(void *parameter) {
  String httpBody;

  // set notification call-back function - Moved to datetimeSyncTask, datetime
  // module's responsibility
  sntp_set_time_sync_notification_cb(time_available);
  configTzTime(TimeConfig::DEFAULT_TIMEZONE_STR, TimeConfig::NTP_SERVER_1,
               TimeConfig::NTP_SERVER_2);

  // --- Fetch Timezone from HTTPS API (moved from original task) ---
  int httpCode = -1;
  client.setCACert(rootCACertificate);
  if (https.begin(client, TimeConfig::TIMEZONE_API_URL)) {
    httpCode = https.GET();
    if (httpCode > 0) {
      Serial.printf("[HTTPS] GET timezone... code: %d\n", httpCode);
      if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
        httpBody = https.getString();
      } else {
        httpBody = "none";
      }
    } else {
      Serial.printf("[HTTPS] GET timezone... failed, error: %s\n",
                    https.errorToString(httpCode).c_str());
      httpBody = "none";
    }
    https.end();
  }
  client.stop();

  // --- Timezone Processing (Simplified) ---
  if (httpBody == "none") {
    Serial.println("Failed to obtain time zone, using default");
    httpBody = TimeConfig::DEFAULT_TIMEZONE_STR;
  } else {
    Serial.println("Timezone from API: " + httpBody);
  }

  setenv("TZ", httpBody.c_str(), 1);
  tzset();

  // --- NTP Time Synchronization (moved from original task) ---
  timeClient.begin();
  timeClient.setTimeOffset(-8 * 3600); // Seattle timezone (PST, UTC-8)

  if (!timeClient.update()) {
    Serial.println("NTPClient update failed!");
  } else {
    Serial.print("NTP Time: ");
    Serial.println(timeClient.getFormattedTime());
  }
  timeClient.end();

  Serial.println("DateTime Sync Task finished");
  vTaskDelete(NULL); // Task completes after one run
}

// Callback function (get's called when time adjusts via NTP) - Moved to
// datetime_module.cpp
static void time_available(struct timeval *t) {
  Serial.println("Got time adjustment from NTP, Write the hardware clock");
  if (amoled_ptr->hasRTC()) {
    amoled_ptr->hwClockWrite();

    // Read hardware clock
    struct tm hwTimeinfo;
    amoled_ptr->getDateTime(&hwTimeinfo);
    Serial.print("Hardware clock :");
    Serial.println(&hwTimeinfo, "%A, %B %d %Y %H:%M:%S");
  }
}

} // namespace DateTime