#include "button_module.h"
#include "constants.h"     // For BOARD_PIXELS_NUM, BoardsConfigure_t, etc.
#include <LilyGo_AMOLED.h> // For LilyGo_Class, BoardsConfigure_t
#include "wifi_module.h"   // Include wifi_module.h to call WiFi::disconnectWiFi()
#include "gui.h"

using namespace Constants;
using namespace ace_button;

namespace Button
{
    AceButton *button = nullptr;         // Declare the button variable
    Adafruit_NeoPixel *pixels = nullptr; // Declare the pixels variable
    static LilyGo_Class *amoled_ptr = nullptr;

    // ... (Static variables and other function implementations from button_module.cpp) ...

    void setupButton(LilyGo_Class &amoled_instance, Adafruit_NeoPixel *pixels_ptr) // Modified setupButton to take amoled instance
    {
        amoled_ptr = &amoled_instance; // Store the amoled instance in the module's scope
        pixels = pixels_ptr;
        const BoardsConfigure_t *boards = amoled_ptr->getBoardsConfigure();
        // Set button on/off charge led , just for test button ,only 1.47 inch amoled available
        if (boards->buttonNum)
        {

            ButtonConfig *buttonConfig;
            button = new AceButton[boards->buttonNum];
            for (int i = 0; i < boards->buttonNum; ++i)
            {
                Serial.print("init button : ");
                Serial.println(i);
                pinMode(boards->pButtons[i], INPUT_PULLUP);
                button[i].init(boards->pButtons[i], HIGH, i);
                buttonConfig = button[i].getButtonConfig();
                buttonConfig->setFeature(ButtonConfig::kFeatureClick);
                buttonConfig->setFeature(ButtonConfig::kFeatureLongPress);
                buttonConfig->setEventHandler(buttonHandleEvent); // buttonHandleEvent is in Button namespace now
            }

            if (boards->pixelsPins != -1)
            {
                pixels->begin(); // This initializes the NeoPixel library.
                pixels->setBrightness(15);

                // Test pixels color
                pixels->setPixelColor(0, pixels->Color(255, 0, 0));
                pixels->show();
                delay(500);
                pixels->setPixelColor(0, pixels->Color(0, 255, 0));
                pixels->show();
                delay(500);
                pixels->setPixelColor(0, pixels->Color(0, 0, 255));
                pixels->show();
                delay(500);
                pixels->clear();
                pixels->show();
            }
        }

        xTaskCreate(buttonHandlerTask, "btn", 5 * 1024, NULL, 12, NULL);
        // Home Button Callback - Keep in button module for now, might move to UI module later
        amoled_ptr->setHomeButtonCallback([](void *ptr)
                                          {
                                     Serial.println("Home key pressed!");
                                     static uint32_t checkMs = 0;
                                     static uint8_t lastBri = 0;
                                     if (millis() > checkMs) {
                                         if (amoled_ptr->getBrightness()) {
                                             lastBri = amoled_ptr->getBrightness();
                                             amoled_ptr->setBrightness(0);
                                         } else {
                                            amoled_ptr->setBrightness(lastBri);
                                         }
                                     }
                                     checkMs = millis() + 200; }, NULL);
    }

    void buttonHandleEvent(AceButton *button, uint8_t eventType, uint8_t buttonState)
    {
        uint8_t id;
        // Print out a message for all events.
        Serial.print(F("handleEvent(): eventType: "));
        Serial.print(AceButton::eventName(eventType));
        Serial.print(F("; buttonState: "));
        Serial.println(buttonState);

        const BoardsConfigure_t *boards = amoled_ptr->getBoardsConfigure(); // You might need to get amoled object passed to this function or make it accessible
        id = amoled_ptr->getBoardID();                                      // Same here, amoled object needed

        switch (eventType)
        {
        case AceButton::kEventClicked:
            if (button->getId() == 0)
            {
                // selectNextItem(); // UI function - we'll deal with this later (UI module)
            }
            else
            {
                if (boards->pmu && id == LILYGO_AMOLED_147)
                {
                    // Toggle CHG led
                    amoled_ptr->setChargingLedMode(
                        amoled_ptr->getChargingLedMode() != XPOWERS_CHG_LED_OFF ? XPOWERS_CHG_LED_OFF : XPOWERS_CHG_LED_ON);
                }
            }
            break;
        case AceButton::kEventLongPressed:

            Serial.println("Enter sleep !");

            // --- Call WiFi::disconnectWiFi() instead of direct WiFi control ---
            disconnectWiFi();

            // --- Task deletion and sleep_flag setting - move to power_module later ---
            // if (vUpdateDateTimeTaskHandler) { vTaskDelete(vUpdateDateTimeTaskHandler); }
            // if (vUpdateCoin360TaskHandler) { vTaskDelete(vUpdateCoin360TaskHandler); }
            // if (vUpdateWeatherTaskHandler) { vTaskDelete(vUpdateWeatherTaskHandler); }
            // sleep_flag = true; // Move sleep_flag to power module

            Serial.println();

            {
                bool touchpad_sleep = true;
                if (id == LILYGO_AMOLED_191 || id == LILYGO_AMOLED_191_SPI)
                {
                    touchpad_sleep = false;
                }
                amoled_ptr->sleep(touchpad_sleep); // Keep display sleep for now - might move to power module later
            }

            if (boards->pixelsPins != -1)
            {
                pixels->clear();
                pixels->show();
            }

            if (boards->pmu && id == LILYGO_AMOLED_147)
            {
                // Set PMU Sleep mode - Keep for now, might move to power module
                amoled_ptr->enableSleep();
                amoled_ptr->clearPMU();
                amoled_ptr->enableWakeup();
                esp_sleep_enable_timer_wakeup(60 * 1000000ULL); // 60S
            }
            else
            {
                // Set BOOT button as wakeup source - Keep for now, might move to power module
                esp_sleep_enable_ext0_wakeup(GPIO_NUM_0, LOW);
            }

            Wire.end(); // Keep for now, might move to power module

            Serial.println("Sleep Start!");
            delay(5000);
            esp_deep_sleep_start();
            Serial.println("This place will never print!");
            break;
        default:
            break;
        }
    }

    void buttonHandlerTask(void *ptr)
    {
        const BoardsConfigure_t *boards = amoled_ptr->getBoardsConfigure();
        while (1)
        {
            for (int i = 0; i < boards->buttonNum; ++i)
            {
                button[i].check();
            }
            delay(5);
        }
        vTaskDelete(NULL);
    }

} // namespace Button