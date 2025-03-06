// src/main.cpp
#include <Arduino.h>
#include "LilyGo_AMOLED.h"
#include "actions.h"
#include "ui.h"
#include "config.h"
#include <lvgl.h>
#include <ostream>
#include <iostream>


LilyGo_AMOLED display; // Create display object

void setup() {
  Serial.begin(115200);
  Serial.println("Starting AmiPixel...");

  display.begin(); // Initialize the display
  if (!display.begin()) { // Check if initialization was successful
    Serial.println("Display initialization failed!");
    while (1) {
      delay(1000); // Stop and blink LED or something to indicate error (if you have an LED)
    }
  }
  display.setBrightness(200)
  Serial.println("Display initialized.");

  // actions_setup(); // Setup for actions (you can add this back later)
  // ui_setup();      // Setup for UI (you can add this back later)


  Serial.println("Setup complete.");
}

void loop() {
  do_action_feed();
  delay(1000); // Wait 1 second
  do_action_play();
  delay(1000);
  do_action_pet();
  delay(1000);
  std::cout << "Looping..." << std::endl; // Keep seeing loop output
  delay(2000); // Loop every 2 seconds (total with action delays)
}