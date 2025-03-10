#ifndef BUTTON_MODULE_H
#define BUTTON_MODULE_H

#include <AceButton.h>
#include <Adafruit_NeoPixel.h>
#include <Arduino.h>
#include <LilyGo_AMOLED.h>

namespace Button {

// --- Function Prototypes ---
void setupButton(LilyGo_Class &amoled,
                 Adafruit_NeoPixel *pixels_ptr); // Pass amoled object for
                                                 // initialization and callbacks
void buttonHandleEvent(ace_button::AceButton *button, uint8_t eventType,
                       uint8_t buttonState);
void buttonHandlerTask(void *ptr);

} // namespace Button

#endif // BUTTON_MODULE_H