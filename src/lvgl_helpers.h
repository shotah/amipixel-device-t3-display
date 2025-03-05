// src/lvgl_helpers.h
#pragma once

#include <Arduino.h>
#include <TFT_eSPI.h>
#include <lvgl.h>

// ---  Screen Resolution Definitions (Make sure these are correct for your T-Display S3 AMOLED) ---
#define screenWidth  240
#define screenHeight 536

void lvgl_driver_init(void);