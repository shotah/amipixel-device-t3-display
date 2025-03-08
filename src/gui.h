
#pragma once

#include <lvgl.h>
#include <time.h>
#include <Arduino.h>
#include "weather.h"

void factoryGUI(Adafruit_NeoPixel *pixels_ptr);
void selectNextItem();
void showCertification(uint32_t delay_ms);