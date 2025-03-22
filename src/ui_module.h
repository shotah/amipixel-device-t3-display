#ifndef UI_MODULE_H
#define UI_MODULE_H

#include "lvgl.h"

// Declare the external gif_cat variable
extern const lv_img_dsc_t gif_cat;

// Function to initialize the UI and display the GIF
void ui_module_init();

#endif // UI_MODULE_H