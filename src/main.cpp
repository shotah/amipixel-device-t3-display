#include "button_module.h"
#include "coin.h"
#include "constants.h"
#include "datetime_module.h"
#include "display_driver.h"
#include "fs_handler.h" // Include fs_handler.h
#include "globals.h"
// #include "gui.h"
#include "weather.h"
#include "wifi_module.h"
#include "zones.h"
#include <AceButton.h>
#include <Adafruit_NeoPixel.h>
#include <Arduino.h>
#include <LV_Helper.h>
#include <LilyGo_AMOLED.h>
#include <WiFi.h>
#include <esp_sntp.h>
#include <esp_wifi.h>
#include <lvgl.h>
#include <time.h>
#include "ui_module.h"

using namespace ace_button;
using namespace Constants;

// --- Global variables (moved to modules where appropriate, keep amoled here
// for now) ---
LilyGo_Class amoled; // Keep amoled object here for now, passed to modules
volatile bool sleep_flag =
    false; // Keep sleep_flag here for now, might move to power module later

// --- Function prototypes (moved to modules where possible, keep UI and main
// loop related here) ---
void buttonHandleEvent(
    AceButton *button, uint8_t eventType,
    uint8_t buttonState); // Prototype for button event handler (defined in
                          // button_module.cpp)
void loop();              // Keep loop prototype
void setup();             // Keep setup prototype

void setup()
{
  Serial.begin(115200);
  delay(7000);
  Serial.println("Starting Factory Example with Modular Code...");

  bool rslt = false;

  rslt = amoled.begin();
  Adafruit_NeoPixel *pixels = NULL;
  const BoardsConfigure_t *boards = amoled.getBoardsConfigure();
  if (boards->pixelsPins != -1)
  {
    pixels = new Adafruit_NeoPixel(BOARD_PIXELS_NUM, boards->pixelsPins,
                                   NEO_GRB + NEO_KHZ800);
    pixels->begin();
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
      Serial.println("The board model cannot be detected, please raise the "
                     "Core Debug Level to an error");
      delay(1000);
    }
  }

  beginLvglHelper(amoled);

  // Initialize and show the GIF UI
  ui_init();

  // --- Initialize Filesystem using FSHandler ---
  if (FSHandler::setupFS())
  {
    Serial.println("[Main] SPIFFS Initialized by FSHandler.");
    FSHandler::listFSContents(); // List contents after successful
                                 // initialization
  }
  else
  {
    Serial.println("[Main] SPIFFS Initialization failed (FSHandler::setupFS() "
                   "returned false)");
  }
  // --- End Filesystem Initialization ---

  // --- Module Setup ---
  Serial.println("Setting up WiFi...");
  WiFiModule::setupWiFi();
  // DateTime::setupDateTime(amoled); // Setup DateTime module
  // Weather::setupWeather();         // Setup Weather module
  // Coin::setupCoin();               // Setup Coin module
  Button::setupButton(amoled, pixels);
  DisplayDriver::setupDisplayDriver(amoled);

  // showCertification(3000);
  // factoryGUI(pixels);

  enableLoopWDT();
  Serial.println("Setup complete. Entering loop().");
}

void loop()
{
  if (sleep_flag)
  {
    return;
  }
  WiFiModule::manageWiFiConnection();
  lv_task_handler();
  delay(1);
}
