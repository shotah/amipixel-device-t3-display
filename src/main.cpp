
#include "constants.h"
#include <LilyGo_AMOLED.h>
#include <LV_Helper.h>
#include <Arduino.h>
#include <time.h>
#include <lvgl.h>
#include <esp_sntp.h>
#include <esp_wifi.h>
#include <WiFi.h>

#include "wifi_module.h"     // Include WiFi module header
#include "datetime_module.h" // Include DateTime module header
#include "weather.h"         // Include Weather module header
#include "coin.h"            // Include Coin module header
#include "button_module.h"   // Include Button module header
#include "display_driver.h"  // Include Display Driver module header
#include "gui.h"             // Include GUI - keep this for UI setup
#include "zones.h"           // Include zones - keep this for timezone

#include <AceButton.h>         // AceButton is used in button_module, but included here for global button definition (if needed temporarily)
#include <Adafruit_NeoPixel.h> // NeoPixel is used in button_module, included here if needed temporarily

using namespace ace_button;
using namespace Constants;

// --- Global variables (moved to modules where appropriate, keep amoled here for now) ---
LilyGo_Class amoled;              // Keep amoled object here for now, passed to modules
volatile bool sleep_flag = false; // Keep sleep_flag here for now, might move to power module later

// --- Function prototypes (moved to modules where possible, keep UI and main loop related here) ---
void buttonHandleEvent(AceButton *button, uint8_t eventType, uint8_t buttonState); // Prototype for button event handler (defined in button_module.cpp)
void loop();                                                                       // Keep loop prototype
void setup();                                                                      // Keep setup prototype

void setup()
{
    Serial.begin(115200);
    delay(3000); // <---- ADD this delay for serial initialization
    Serial.println("Starting Factory Example with Modular Code...");

    bool rslt = false;

    // Automatically determine the access device
    rslt = amoled.begin();
    // --- Global variables ---  (Declaration can be here, instantiation MUST be after amoled.begin())
    Adafruit_NeoPixel *pixels = NULL; // Global declaration - OK here
    const BoardsConfigure_t *boards = amoled.getBoardsConfigure();
    if (boards->pixelsPins != -1)
    {
        pixels = new Adafruit_NeoPixel(BOARD_PIXELS_NUM, boards->pixelsPins, NEO_GRB + NEO_KHZ800);
        pixels->begin(); // This initializes the NeoPixel library.
        pixels->setBrightness(15);
    }
    Serial.println("============================================");
    Serial.print("    Board Name:LilyGo AMOLED ");
    Serial.println(amoled.getName());
    Serial.println("============================================");

    if (!rslt)
    {
        while (1)
        {
            Serial.println("The board model cannot be detected, please raise the Core Debug Level to an error");
            delay(1000);
        }
    }

    // Register lvgl helper
    beginLvglHelper(amoled);

    // --- Module Setup ---
    setupWiFi();                               // Setup WiFi module
    DateTime::setupDateTime(amoled);           // Setup DateTime module
    Weather::setupWeather();                   // Setup Weather module
    Coin::setupCoin();                         // Setup Coin module
    Button::setupButton(amoled, pixels);       // Setup Button module, pass amoled instance
    DisplayDriver::setupDisplayDriver(amoled); // Setup Display Driver module, pass amoled

    // Show certificate image
    showCertification(3000);

    // Draw Factory GUI
    factoryGUI(pixels);

    // --- WiFi Connection (moved to wifi_module.cpp for setup, but connection can start here) ---
    connectWiFi();

    // Enable Watchdog
    enableLoopWDT();
    Serial.println("Setup complete. Entering loop().");
}

uint32_t last_check_connected = 0;

void loop()
{
    if (sleep_flag)
    {
        return; // If sleep_flag is set, exit loop (sleep handled in button module)
    }
    if (last_check_connected < millis())
    {
        if (WiFi.status() != WL_CONNECTED)
        {
            connectWiFi(); // Reconnect WiFi if disconnected (using wifi_module function)
            Serial.println("Reconnecting WiFi in loop...");
        }
        last_check_connected = millis() + 5000;
    }

    lv_task_handler();
    delay(1);
}
