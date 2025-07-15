#include "coin.h" // Updated include to "coin_module.h" (matching the header filename)
#include "cJSON.h"     // Include cJSON library header
#include "constants.h" // For APIKeys, TimeConfig, MessageIDs, CoinMarketCapApiSubsribe, CoinMarketCapApiDataStream
#include "root_ca.h"
#include "wifi_module.h" // Include for WiFiMessagesEnum
#include <HTTPClient.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <lvgl.h> // For lv_msg and message IDs

using namespace Constants;

namespace Coin {
// --- Static variables ---
CoinApiData coinApi[4]; // Define global coinApi array instance (within the
                        // namespace now)
static WiFiClientSecure client;
static HTTPClient https;
static String httpBody; // Static, module-level httpBody

// --- Static configuration data (moved from global scope, if not already
// static) ---
static CoinMarketCapApiSubsribe coinSubsribe[4] = {
    {1, NULL, "BTC"},    // {id, src_img, name} - BTC, ID = 1 (example)
    {1027, NULL, "ETH"}, // {id, src_img, name} - ETH, ID = 1027 (example)
    {2, NULL, "LTC"},    // {id, src_img, name} - LTC, ID = 2 (example)
    {74, NULL, "DOGE"}   // {id, src_img, name} - DOGE, ID = 74 (example)
};
static CoinMarketCapApiDataStream coinData[4]; // Static, module-level coinData

// --- Static Function Prototypes ---
static void coinUpdateTask(void *ptr); // Static, internal coin update task

// Simple callback-based approach replacing LVGL messaging
static bool wifi_connected = false;

void setupCoin() {
  // No messaging setup needed - will use direct function calls
  Serial.println("Coin module initialized");
}

void onWifiConnected() {
  // Called directly when WiFi connects (replaces messaging)
  Serial.println("Coin Module: WiFi connected, starting coin updates");
  wifi_connected = true;
  static TaskHandle_t coinTaskHandle =
      NULL; // Static handle to avoid re-creation
  if (coinTaskHandle == NULL) {
    xTaskCreate(coinUpdateTask, "coinUpdate", 10 * 1024, NULL, 9,
                &coinTaskHandle); // Example priority, adjust as needed -
                                  // lower than weather/datetime
  }
}

static void coinUpdateTask(
    void *ptr) // Made static and void (no ptr parameter needed for now)
{
  cJSON *root = nullptr; // Modern C++ initialization to nullptr
  bool done = false;
  bool findHeaders = false;

  // --- Prepare CoinMarketCap API Request ---
  String url = "/v1/cryptocurrency/quotes/latest?symbol=";
  int counter = sizeof(Coin::coinSubsribe) / sizeof(Coin::coinSubsribe[0]);
  for (int i = 0; i < counter; ++i) {
    url += Coin::coinSubsribe[i].name;
    if (i != counter - 1) {
      url += ",";
    }
  }
  url += "&convert=USD";

  Coin::client.setCACert(CoinMarketCapApiRootCA);

  if (Coin::client.connect(
          TimeConfig::COINMARKETCAP_API_HOST,
          443)) // Use client and https, and url.c_str() for std::string
  {
    Coin::client.println("GET " + url + " HTTP/1.1");
    Coin::client.println("Host: " + String(TimeConfig::COINMARKETCAP_API_HOST));
    Coin::client.println("User-Agent: arduino/1.0.0");
    Coin::client.println("Accepts: application/json");
    Coin::client.print("X-CMC_PRO_API_KEY: ");
    Coin::client.println(APIKeys::COINMARKETCAP);
    Coin::client.println();

    uint32_t now = millis();
    while (millis() - now < 3000) // Timeout for data reception
    {
          while (Coin::client.available()) {
      char c = Coin::client.read();
        if (!findHeaders) {
          if (c == '{') {
            findHeaders = true;
            Coin::httpBody = c; // Start capturing body from '{'
          }
        } else {
          if (c == '\r') // Stop reading at the end of the JSON body (assuming
                         // it's terminated by \r after headers)
          {
            break;
          } else {
            Coin::httpBody += c; // Append to the body
          }
        }
        delay(1);
      }
    }
    Coin::client.stop();
    Serial.println(Coin::httpBody); // Print the received JSON body (for debugging)

    // --- JSON Parsing ---
    root = cJSON_Parse(Coin::httpBody.c_str());
    if (root != nullptr) // Check for null after parsing
    {
      cJSON *data = cJSON_GetObjectItem(root, "data");
      if (data != nullptr) {
        int size = cJSON_GetArraySize(data);
        for (int i = 0; i < size; i++) {
          cJSON *coinSymbol = cJSON_GetObjectItem(
              data,
              coinSubsribe[i]
                  .name); // Use coinSubsribe[i].name to access coin symbol
          if (coinSymbol != nullptr) {
            cJSON *quote = cJSON_GetObjectItem(coinSymbol, "quote");
            if (quote != nullptr) {
              cJSON *currencyUsd = cJSON_GetObjectItem(quote, "USD");
              if (currencyUsd != nullptr) {
                cJSON *price = cJSON_GetObjectItem(currencyUsd, "price");
                cJSON *percent_change_24h =
                    cJSON_GetObjectItem(currencyUsd, "percent_change_24h");

                if (price != nullptr && percent_change_24h != nullptr) {
                  Coin::coinApi[i].price_usd = price->valuedouble;
                  Coin::coinApi[i].percent_change_24h =
                      percent_change_24h->valuedouble;

                  Serial.print(
                      coinSubsribe[i]
                          .name); // Use coinSubsribe[i].name for coin name
                  Serial.print(":");
                  Serial.print("\t");
                  Serial.print("Price:");
                  Serial.println(Coin::coinApi[i].price_usd);
                  Serial.print("\t");
                  Serial.print("24H Change:");
                  Serial.println(Coin::coinApi[i].percent_change_24h);

                  // TODO: Replace with direct callback or event system
                  // For now, just log the coin data
                  Serial.println("Coin data updated successfully");
                  done = true;
                } else {
                  Serial.println("Error parsing price or percent_change_24h");
                }
              } else {
                Serial.println("Error parsing USD currency data");
              }
            } else {
              Serial.println("Error parsing quote data");
            }
          } else {
            Serial.printf(
                "Error parsing data for coin: %s\n",
                coinSubsribe[i].name); // Use coinSubsribe[i].name for coin name
          }
        }
      } else {
        Serial.println("Error parsing 'data' array");
      }
    } else {
      Serial.println("JSON parse failed!"); // More specific error message
    }
  } else {
    Serial.println("CoinMarketCap connection failed");
  }

  if (root != nullptr) // Check for nullptr before freeing
  {
    cJSON_Delete(
        root); // Use cJSON_Delete instead of cJSON_free for modern cJSON
  }
  Serial.println("Coin Update Task finished");
  vTaskDelete(NULL); // Task completes after one run
}

} // namespace Coin