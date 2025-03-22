#ifndef UI_MODULE_H
#define UI_MODULE_H

#include "lvgl.h"
#include "avatar_state.h" // Include the avatar state module

// Forward declarations of UI elements
extern lv_obj_t *screen_main;
extern lv_obj_t *btn_feed;
extern lv_obj_t *btn_play;
extern lv_obj_t *img_avatar;

// Global instance of the avatar state
extern avatar_state_t avatar_data;

// Function to initialize the UI
void ui_init(void);

// Event handlers for buttons
void btn_feed_cb(lv_event_t *e);
void btn_play_cb(lv_event_t *e);

// Function to set the avatar's GIF based on its state
void update_avatar_gif(void);

// Function to track actions (you can expand this)
void track_action(const char *action_name);

#endif // UI_MODULE_H