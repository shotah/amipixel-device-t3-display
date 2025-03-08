/**
 * @file      gui.h
 * @author    Lewis He (lewishe@outlook.com)
 * @license   MIT
 * @copyright Copyright (c) 2023  Shenzhen Xin Yuan Electronic Technology Co., Ltd
 * @date      2023-07-14
 *
 */

#pragma once

#include <lvgl.h>
#include <time.h>
#include <Arduino.h>
#include "weather.h"

void factoryGUI(Adafruit_NeoPixel *pixels_ptr);
void selectNextItem();
void showCertification(uint32_t delay_ms);