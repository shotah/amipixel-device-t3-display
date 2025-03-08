#include "coin.h"        // Updated include to "coin_module.h" (matching the header filename)
#include "wifi_module.h" // Include for WiFiMessagesEnum
#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include "root_ca.h"
#include "cJSON.h"     // Include cJSON library header
#include <lvgl.h>      // For lv_msg and message IDs
#include "constants.h" // For APIKeys, TimeConfig, MessageIDs, CoinMarketCapApiSubsribe, CoinMarketCapApiDataStream

using namespace Constants;

namespace Coin
{
    // --- Static variables ---
    CoinApiData coinApi[4]; // Define global coinApi array instance (within the namespace now)
    static WiFiClientSecure client;
    static HTTPClient https;
    static String httpBody; // Static, module-level httpBody

    // --- Static configuration data (moved from global scope, if not already static) ---
    static CoinMarketCapApiSubsribe coinSubsribe[4] = {
        {1, NULL, "BTC"},    // {id, src_img, name} - BTC, ID = 1 (example)
        {1027, NULL, "ETH"}, // {id, src_img, name} - ETH, ID = 1027 (example)
        {2, NULL, "LTC"},    // {id, src_img, name} - LTC, ID = 2 (example)
        {74, NULL, "DOGE"}   // {id, src_img, name} - DOGE, ID = 74 (example)
    };
    static CoinMarketCapApiDataStream coinData[4]; // Static, module-level coinData

    // --- Static Function Prototypes ---
    static void handleWifiMessages(void *subscriber_ptr, lv_msg_t *msg); // Corrected message handler signature
    static void coinUpdateTask(void *ptr);                               // Static, internal coin update task

    void setupCoin()
    {
        // Subscribe to WIFI_GOT_IP_MSG to start coin updates when WiFi connects
        lv_msg_subscribe(static_cast<uint32_t>(MessageIDs::WiFiMessagesEnum::WIFI_GOT_IP_MSG), handleWifiMessages, nullptr);
    }

    static void handleWifiMessages(void *subscriber_ptr, lv_msg_t *msg)
    {
        uint32_t msg_id = lv_msg_get_id(msg); // Get the message ID from lv_msg_t

        if (msg_id == static_cast<uint32_t>(MessageIDs::WiFiMessagesEnum::WIFI_GOT_IP_MSG))
        {
            Serial.println("Coin Module received WIFI_GOT_IP_MSG");
            // Start the coin update task when WiFi is connected
            static TaskHandle_t coinTaskHandle = NULL; // Static handle to avoid re-creation
            if (coinTaskHandle == NULL)
            {
                xTaskCreate(coinUpdateTask, "coinUpdate", 10 * 1024, NULL, 9, &coinTaskHandle); // Example priority, adjust as needed - lower than weather/datetime
            }
        }
    }

    static void coinUpdateTask(void *ptr) // Made static and void (no ptr parameter needed for now)
    {
        cJSON *root = nullptr; // Modern C++ initialization to nullptr
        bool done = false;
        bool findHeaders = false;

        // --- Prepare CoinMarketCap API Request ---
        String url = "/v1/cryptocurrency/quotes/latest?symbol=";
        int counter = sizeof(coinSubsribe) / sizeof(coinSubsribe[0]);
        for (int i = 0; i < counter; ++i)
        {
            url += coinSubsribe[i].name;
            if (i != counter - 1)
            {
                url += ",";
            }
        }
        url += "&convert=USD";

        client.setCACert(CoinMarketCapApiRootCA);

        if (client.connect(TimeConfig::COINMARKETCAP_API_HOST, 443)) // Use client and https, and url.c_str() for std::string
        {
            client.println("GET " + url + " HTTP/1.1");
            client.println("Host: " + String(TimeConfig::COINMARKETCAP_API_HOST));
            client.println("User-Agent: arduino/1.0.0");
            client.println("Accepts: application/json");
            client.print("X-CMC_PRO_API_KEY: ");
            client.println(APIKeys::COINMARKETCAP);
            client.println();

            uint32_t now = millis();
            while (millis() - now < 3000) // Timeout for data reception
            {
                while (client.available())
                {
                    char c = client.read();
                    if (!findHeaders)
                    {
                        if (c == '{')
                        {
                            findHeaders = true;
                            httpBody = c; // Start capturing body from '{'
                        }
                    }
                    else
                    {
                        if (c == '\r') // Stop reading at the end of the JSON body (assuming it's terminated by \r after headers)
                        {
                            break;
                        }
                        else
                        {
                            httpBody += c; // Append to the body
                        }
                    }
                    delay(1);
                }
            }
            client.stop();
            Serial.println(httpBody); // Print the received JSON body (for debugging)

            // --- JSON Parsing ---
            root = cJSON_Parse(httpBody.c_str());
            if (root != nullptr) // Check for null after parsing
            {
                cJSON *data = cJSON_GetObjectItem(root, "data");
                if (data != nullptr)
                {
                    int size = cJSON_GetArraySize(data);
                    for (int i = 0; i < size; i++)
                    {
                        cJSON *coinSymbol = cJSON_GetObjectItem(data, coinSubsribe[i].name); // Use coinSubsribe[i].name to access coin symbol
                        if (coinSymbol != nullptr)
                        {
                            cJSON *quote = cJSON_GetObjectItem(coinSymbol, "quote");
                            if (quote != nullptr)
                            {
                                cJSON *currencyUsd = cJSON_GetObjectItem(quote, "USD");
                                if (currencyUsd != nullptr)
                                {
                                    cJSON *price = cJSON_GetObjectItem(currencyUsd, "price");
                                    cJSON *percent_change_24h = cJSON_GetObjectItem(currencyUsd, "percent_change_24h");

                                    if (price != nullptr && percent_change_24h != nullptr)
                                    {
                                        coinApi[i].price_usd = price->valuedouble;
                                        coinApi[i].percent_change_24h = percent_change_24h->valuedouble;

                                        Serial.print(coinSubsribe[i].name); // Use coinSubsribe[i].name for coin name
                                        Serial.print(":");
                                        Serial.print("\t");
                                        Serial.print("Price:");
                                        Serial.println(coinApi[i].price_usd);
                                        Serial.print("\t");
                                        Serial.print("24H Change:");
                                        Serial.println(coinApi[i].percent_change_24h);

                                        lv_msg_send(MessageIDs::COIN_MSG_ID, &coinApi[i]); // Send coin data via lv_msg for each coin
                                        done = true;
                                    }
                                    else
                                    {
                                        Serial.println("Error parsing price or percent_change_24h");
                                    }
                                }
                                else
                                {
                                    Serial.println("Error parsing USD currency data");
                                }
                            }
                            else
                            {
                                Serial.println("Error parsing quote data");
                            }
                        }
                        else
                        {
                            Serial.printf("Error parsing data for coin: %s\n", coinSubsribe[i].name); // Use coinSubsribe[i].name for coin name
                        }
                    }
                }
                else
                {
                    Serial.println("Error parsing 'data' array");
                }
            }
            else
            {
                Serial.println("JSON parse failed!"); // More specific error message
            }
        }
        else
        {
            Serial.println("CoinMarketCap connection failed");
        }

        if (root != nullptr) // Check for nullptr before freeing
        {
            cJSON_Delete(root); // Use cJSON_Delete instead of cJSON_free for modern cJSON
        }
        Serial.println("Coin Update Task finished");
        vTaskDelete(NULL); // Task completes after one run
    }

} // namespace Coin