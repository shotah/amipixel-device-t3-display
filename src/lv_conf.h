/**
 * @file lv_conf.h
 * LVGL Configuration for ESP32S3 T-Display-AMOLED
 * Optimized for low DRAM usage with PSRAM allocation
 */

#ifndef LV_CONF_H
#define LV_CONF_H

/* CRITICAL: Disable ARM optimizations for ESP32S3 (Xtensa architecture) */
#define LV_USE_DRAW_SW_ASM        LV_DRAW_SW_ASM_NONE

/* Basic LVGL settings */
#define LV_COLOR_DEPTH 16
#define LV_USE_STDLIB_MALLOC     LV_STDLIB_BUILTIN
#define LV_USE_STDLIB_STRING     LV_STDLIB_BUILTIN
#define LV_USE_STDLIB_SPRINTF    LV_STDLIB_BUILTIN

/* Memory settings optimized for ESP32S3 - Use PSRAM for large allocations */
#define LV_MEM_SIZE (128 * 1024U)  /* Reduced to 128KB for better DRAM usage */

/* Enable PSRAM usage for ESP32S3 */
#define LV_MEM_POOL_INCLUDE <esp_heap_caps.h>
#define LV_MEM_POOL_ALLOC(size) heap_caps_malloc(size, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT)
#define LV_MEM_POOL_FREE(ptr) heap_caps_free(ptr)

/* Optimize drawing buffers for lower memory usage */
#define LV_DRAW_BUF_SIZE_PREFERRED   (10 * 1024)   /* Smaller draw buffer */
#define LV_DRAW_BUF_ALIGN            4

/* Enable features needed by the project - minimal set */
#define LV_USE_CANVAS 1
#define LV_USE_CHART 1
#define LV_USE_FLEX 1
#define LV_USE_GRID 1

/* Enable widgets used in project - only what we need */
#define LV_USE_SLIDER 1
#define LV_USE_BAR 1
#define LV_USE_LABEL 1
#define LV_USE_BUTTON 1
#define LV_USE_IMG 1
#define LV_USE_TABVIEW 1
#define LV_USE_SPINNER 1
#define LV_USE_ARC 1        /* Required by spinner */
#define LV_USE_TEXTAREA 1   /* Required by spinbox */

/* Disable unused widgets to save memory - but keep dependencies */
#define LV_USE_ANIMIMG 0
#define LV_USE_BTNMATRIX 0
#define LV_USE_CALENDAR 0
#define LV_USE_CHECKBOX 0
#define LV_USE_DROPDOWN 0
#define LV_USE_KEYBOARD 0
#define LV_USE_LED 0
#define LV_USE_LINE 0
#define LV_USE_LIST 0
#define LV_USE_MENU 0
#define LV_USE_METER 0
#define LV_USE_MSGBOX 0
#define LV_USE_ROLLER 0
#define LV_USE_SCALE 0
#define LV_USE_SWITCH 0
#define LV_USE_TABLE 0
#define LV_USE_WIN 0

/* Disable unused themes to save flash and RAM */
#define LV_USE_THEME_DEFAULT 1
#define LV_USE_THEME_BASIC 0
#define LV_USE_THEME_MONO 0

/* Enable GIF support but optimize */
#define LV_USE_GIF 1

/* Font settings - disable unused fonts */
#define LV_FONT_MONTSERRAT_8  0
#define LV_FONT_MONTSERRAT_10 0
#define LV_FONT_MONTSERRAT_12 0
#define LV_FONT_MONTSERRAT_14 1  /* Keep only needed sizes */
#define LV_FONT_MONTSERRAT_16 1
#define LV_FONT_MONTSERRAT_18 1  /* Required by ui_module.cpp and gui.cpp */
#define LV_FONT_MONTSERRAT_20 0
#define LV_FONT_MONTSERRAT_22 0
#define LV_FONT_MONTSERRAT_24 0
#define LV_FONT_MONTSERRAT_26 0
#define LV_FONT_MONTSERRAT_28 0
#define LV_FONT_MONTSERRAT_30 0
#define LV_FONT_MONTSERRAT_32 0
#define LV_FONT_MONTSERRAT_34 0
#define LV_FONT_MONTSERRAT_36 0
#define LV_FONT_MONTSERRAT_38 0
#define LV_FONT_MONTSERRAT_40 0
#define LV_FONT_MONTSERRAT_42 0
#define LV_FONT_MONTSERRAT_44 0
#define LV_FONT_MONTSERRAT_46 0
#define LV_FONT_MONTSERRAT_48 0

/* Logging - reduce in production */
#define LV_USE_LOG 1
#define LV_LOG_LEVEL LV_LOG_LEVEL_WARN  /* Only warnings and errors */

/* Performance optimizations */
#define LV_USE_PERF_MONITOR 0
#define LV_USE_MEM_MONITOR 0

#endif /*LV_CONF_H*/ 