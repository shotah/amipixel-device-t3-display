
#pragma once

#include "weather.h"
#include <Adafruit_NeoPixel.h>
#include <Arduino.h>
#include <lvgl.h>
#include <time.h>

void factoryGUI(Adafruit_NeoPixel *pixels_ptr);
void selectNextItem();
void showCertification(uint32_t delay_ms);