// ui.cpp
#include "ui.h"
#include <lvgl.h>

void ui_init() {
    // 1. Get the current screen
    lv_obj_t * screen = lv_scr_act();

    // 2. Set background color (example: light gray)
    lv_obj_set_style_bg_color(screen, lv_color_hex(0xEEEEEE), LV_PART_MAIN);

    // 3. Create Avatar Area Placeholder (rectangle with border)
    lv_obj_t * avatar_area = lv_obj_create(screen);
    lv_obj_set_size(avatar_area, 120, 120); // Example size: 120x120 pixels
    lv_obj_align(avatar_area, LV_ALIGN_CENTER, 0, -30); // Center, a bit ऊपर
    lv_obj_set_style_border_width(avatar_area, 2, LV_PART_MAIN); // Border
    lv_obj_set_style_border_color(avatar_area, lv_color_black(), LV_PART_MAIN); // Black border

    // 4. Create Needs/Mood Labels (for now, just placeholders)
    lv_obj_t * hunger_label = lv_label_create(screen);
    lv_label_set_text(hunger_label, "Hunger: ");
    lv_obj_align(hunger_label, LV_ALIGN_TOP_MID, -50, 10); // Top-mid, left a bit

    lv_obj_t * happiness_label = lv_label_create(screen);
    lv_label_set_text(happiness_label, "Happiness: ");
    lv_obj_align(happiness_label, LV_ALIGN_TOP_MID, 50, 10); // Top-mid, right a bit

    lv_obj_t * mood_label = lv_label_create(screen);
    lv_label_set_text(mood_label, "Mood: ");
    lv_obj_align_to(mood_label, hunger_label, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 5); // Below hunger, aligned left

    // 5. Create Action Buttons (Text buttons for now)
    lv_obj_t * feed_button = lv_btn_create(screen);
    lv_obj_set_size(feed_button, 80, 30); // Button size
    lv_obj_align(feed_button, LV_ALIGN_BOTTOM_LEFT, 20, -20); // Bottom-left

    lv_obj_t * feed_label = lv_label_create(feed_button);
    lv_label_set_text(feed_label, "Feed");
    lv_obj_center(feed_label); // Center label on button

    lv_obj_t * play_button = lv_btn_create(screen);
    lv_obj_set_size(play_button, 80, 30);
    lv_obj_align(play_button, LV_ALIGN_BOTTOM_MID, 0, -20); // Bottom-mid

    lv_obj_t * play_label = lv_label_create(play_button);
    lv_label_set_text(play_label, "Play");
    lv_obj_center(play_label);

    lv_obj_t * pet_button = lv_btn_create(screen);
    lv_obj_set_size(pet_button, 80, 30);
    lv_obj_align(pet_button, LV_ALIGN_BOTTOM_RIGHT, -20, -20); // Bottom-right

    lv_obj_t * pet_label = lv_label_create(pet_button);
    lv_label_set_text(pet_label, "Pet");
    lv_obj_center(pet_label);
}