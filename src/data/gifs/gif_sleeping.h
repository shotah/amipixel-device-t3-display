#pragma once

#ifdef __has_include
#if __has_include("lvgl.h")
#ifndef LV_LVGL_H_INCLUDE_SIMPLE
#define LV_LVGL_H_INCLUDE_SIMPLE
#endif
#endif
#endif

#if defined(LV_LVGL_H_INCLUDE_SIMPLE)
#include "lvgl.h"
#else
#include "lvgl.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Simple stick figure animation: sleeping
 * Generated from string patterns - easy to edit!
 * Size: 60x40 pixels
 * Frames: 6
 * 
 * Frame 0 is always the base frame for consistency.
 */

// External declaration of the GIF data
extern const lv_img_dsc_t gif_sleeping;

#ifdef __cplusplus
}
#endif
