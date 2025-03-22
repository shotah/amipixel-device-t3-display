#include "ui_module.h"
#include "lvgl.h"

// You might need to include the header file where gif_cat is actually defined
// For example, if your converter.py generates cat.h, you would include it here:
// #include "cat.h"

void ui_module_init()
{
    lv_obj_t *scr = lv_scr_act(); // Get the current screen

    // Create a GIF object
    lv_obj_t *gif_obj = lv_gif_create(scr);

    // Set the source of the GIF
    lv_gif_set_src(gif_obj, &gif_cat);

    // Align the GIF to the center of the screen (optional)
    lv_obj_align(gif_obj, LV_ALIGN_CENTER, 0, 0);

    // You might want to set a size for the GIF object if needed
    // lv_obj_set_size(gif_obj, lv_img_dsc_get_width(&gif_cat), lv_img_dsc_get_height(&gif_cat));
}