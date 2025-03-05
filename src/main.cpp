// src/main.cpp
#include <Arduino.h>
#include <TFT_eSPI.h> // Include TFT_eSPI library
#include <lvgl.h>
#include "ui.h"
#include "actions.h"
#include "lvgl_helpers.h" // Include lvgl_helpers.h
#include <ostream>
#include <iostream>

TFT_eSPI tft = TFT_eSPI(); /* TFT instance - Declare TFT_eSPI object globally */


void setup() {
  Serial.begin(115200);
  Serial.println("Starting AmiPixel...");

  lv_init(); // Initialize LVGL

  /* Initialize дисплей and input devices(Touch) - Use lvgl_driver_init from lvgl_helpers */
  lvgl_driver_init();

  ui_init(); // Initialize your UI

  // --- Test Action Calls (for now) ---
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