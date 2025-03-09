#include "Weather.h" // Updated include to "Weather.h" (matching the header filename)
#include "constants.h"
#include "root_ca.h"
#include "wifi_module.h" // Include for WiFiMessagesEnum
#include <HTTPClient.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <cJSON.h>
#include <lvgl.h> // For lv_msg and message IDs

using namespace Constants;

namespace Weather {
// --- Static variables ---
WeatherApiData weatherApi; // Definition of the global weatherApi instance
                           // (within the namespace now)
static WiFiClientSecure client;
static HTTPClient https;

// --- Static Function Prototypes ---
static void
handleWifiMessages(void *subscriber_ptr,
                   lv_msg_t *msg); // Corrected message handler signature
static void
weatherUpdateTask(void *ptr); // Static, internal weather update task

void setupWeather() {
  // Subscribe to WIFI_GOT_IP_MSG to start weather updates when WiFi connects
  lv_msg_subscribe(
      static_cast<uint32_t>(MessageIDs::WiFiMessagesEnum::WIFI_GOT_IP_MSG),
      handleWifiMessages, nullptr);
}

static void handleWifiMessages(void *subscriber_ptr, lv_msg_t *msg) {
  uint32_t msg_id = lv_msg_get_id(msg); // Get the message ID from lv_msg_t

  if (msg_id ==
      static_cast<uint32_t>(MessageIDs::WiFiMessagesEnum::WIFI_GOT_IP_MSG)) {
    Serial.println("Weather Module received WIFI_GOT_IP_MSG");
    // Start the weather update task when WiFi is connected
    static TaskHandle_t weatherTaskHandle =
        NULL; // Static handle to avoid re-creation
    if (weatherTaskHandle == NULL) {
      xTaskCreate(weatherUpdateTask, "weatherUpdate", 10 * 1024, NULL, 10,
                  &weatherTaskHandle); // Example priority, adjust as needed
    }
  }
}

static void weatherUpdateTask(
    void *ptr) // Made static and void (no ptr parameter needed for now)
{
  int httpCode = -1;
  cJSON *root = nullptr; // Modern C++ initialization to nullptr
  bool done = false;

  // --- Build URL (Corrected to use constants from constants.h) ---
  std::string url = "https://api.openweathermap.org/data/2.5/weather?lat=";
  url += std::to_string(
      WeatherLocation::LATITUDE); // Access LATITUDE from constants.h
  url += "&lon=";
  url += std::to_string(
      WeatherLocation::LONGITUDE); // Access LONGITUDE from constants.h

  if constexpr (WeatherLocation::USE_FAHRENHEIT) {
    url += "&units=imperial";
  } else if constexpr (WeatherLocation::USE_CELSIUS) {
    url += "&units=metric";
  }
  url += "&appid=";
  url += APIKeys::OPENWEATHERMAP;

  client.setCACert(
      OpenWeatherRootCA); // Assuming OpenWeatherRootCA is defined in root_ca.h

  if (https.begin(
          client,
          url.c_str())) // Use client and https, and url.c_str() for std::string
  {
    httpCode = https.GET();
    if (httpCode > 0) {
      Serial.printf("[HTTPS] GET... code: %d\n", httpCode);
      if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
        std::string httpBody =
            https.getString().c_str(); // Get string as std::string

        // --- JSON Parsing ---
        root = cJSON_Parse(httpBody.c_str());
        if (root != nullptr) // Check for null after parsing
        {
          // ... (JSON parsing logic - mostly the same, but using
          // weatherApi.city = ... and weatherApi.description = ... )
          cJSON *cityJson = cJSON_GetObjectItem(root, "name");
          if (cityJson != nullptr) {
            weatherApi.city =
                cityJson
                    ->valuestring; // Assign to weatherApi.city (std::string)
          }

          cJSON *weather = cJSON_GetObjectItem(root, "weather");
          if (weather != nullptr) {
            cJSON *weatherItem = cJSON_GetArrayItem(weather, 0);
            if (weatherItem != nullptr) {
              cJSON *descriptionJson =
                  cJSON_GetObjectItem(weatherItem, "description");
              if (descriptionJson != nullptr) {
                weatherApi.description =
                    descriptionJson
                        ->valuestring; // Assign to weatherApi.description
                                       // (std::string)
              }
            }
          }

          cJSON *main = cJSON_GetObjectItem(root, "main");
          if (main != nullptr) {
            cJSON *temperature = cJSON_GetObjectItem(main, "temp");
            if (temperature != nullptr) {
              weatherApi.temperature = temperature->valuedouble;
            }
            cJSON *temp_min = cJSON_GetObjectItem(main, "temp_min");
            if (temp_min != nullptr) {
              weatherApi.temp_min = temp_min->valuedouble;
            }
            cJSON *temp_max = cJSON_GetObjectItem(main, "temp_max");
            if (temp_max != nullptr) {
              weatherApi.temp_max = temp_max->valuedouble;
            }
            cJSON *pressure = cJSON_GetObjectItem(main, "pressure");
            if (pressure != nullptr) {
              weatherApi.pressure = pressure->valuedouble;
            }
            cJSON *humidity = cJSON_GetObjectItem(main, "humidity");
            if (humidity != nullptr) {
              weatherApi.humidity = humidity->valuedouble;
            }
          }

          Serial.println("Weather:");
          Serial.print("\tCity:");
          Serial.println(
              weatherApi.city.c_str()); // Use c_str() to print std::string
          Serial.print("\tdescription:");
          Serial.println(weatherApi.description
                             .c_str()); // Use c_str() to print std::string
          Serial.print("\ttemperature:");
          Serial.println(weatherApi.temperature);
          Serial.print("\ttemp_min:");
          Serial.println(weatherApi.temp_min);
          Serial.print("\ttemp_max:");
          Serial.println(weatherApi.temp_max);
          Serial.print("\tpressure:");
          Serial.println(weatherApi.pressure);
          Serial.print("\thumidity:");
          Serial.println(weatherApi.humidity);

          lv_msg_send(MessageIDs::WEATHER_MSG_ID,
                      &weatherApi); // Send weather data via lv_msg

          done = true;
        } else {
          Serial.println("JSON parse failed!"); // More specific error message
        }
      }
    } else {
      Serial.printf("[HTTPS] GET... failed, error: %s\n",
                    https.errorToString(httpCode).c_str());
    }
    https.end();
  }
  client.stop();

  if (root != nullptr) // Check for nullptr before freeing
  {
    cJSON_Delete(
        root); // Use cJSON_Delete instead of cJSON_free for modern cJSON
  }
  Serial.println("Weather Update Task finished");
  vTaskDelete(NULL); // Task completes after one run
}

} // namespace Weather