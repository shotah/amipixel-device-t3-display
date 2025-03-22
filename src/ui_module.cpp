#include "ui_module.h"
#include "gif_cat.c"    // Adjust the path to the correct location of gif_cat.c
#include "gif_rabbit.c" // Example of another GIF
#include "esp_log.h"

static const char *TAG = "UI_MODULE";

// Global instance of the avatar state
avatar_state_t avatar_data;

// Screen
lv_obj_t *screen_main;

// Buttons
lv_obj_t *btn_feed;
lv_obj_t *btn_play;

// Image to display the avatar GIF
lv_obj_t *img_avatar;

// Function to initialize the UI
void ui_init(void)
{
    lv_init(); // Make sure LVGL is initialized elsewhere

    // Load avatar state from NVS
    if (!avatar_state_load(&avatar_data))
    {
        avatar_state_init(&avatar_data);
        avatar_state_save(&avatar_data); // Save initial state
    }
    else
    {
        ESP_LOGI(TAG, "Avatar state loaded successfully.");
    }

    // Create the main screen
    screen_main = lv_obj_create(NULL);
    lv_scr_load(screen_main);

    // Create the avatar image object
    img_avatar = lv_img_create(screen_main);
    lv_obj_align(img_avatar, LV_ALIGN_CENTER, 0, -20);
    update_avatar_gif(); // Set initial GIF based on loaded state

    // Create "Feed" button
    btn_feed = lv_btn_create(screen_main);
    lv_obj_set_size(btn_feed, 80, 30);
    lv_obj_align(btn_feed, LV_ALIGN_BOTTOM_LEFT, 10, -10);
    lv_obj_add_event_cb(btn_feed, btn_feed_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_t *label_feed = lv_label_create(btn_feed);
    lv_label_set_text(label_feed, "Feed");
    lv_obj_center(label_feed);

    // Create "Play" button
    btn_play = lv_btn_create(screen_main);
    lv_obj_set_size(btn_play, 80, 30);
    lv_obj_align(btn_play, LV_ALIGN_BOTTOM_RIGHT, -10, -10);
    lv_obj_add_event_cb(btn_play, btn_play_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_t *label_play = lv_label_create(btn_play);
    lv_label_set_text(label_play, "Play");
    lv_obj_center(label_play);
}

// Event handler for the "Feed" button
void btn_feed_cb(lv_event_t *e)
{
    LV_UNUSED(e);
    track_action("Feed");
    if (avatar_data.hunger < 100)
    {
        avatar_data.hunger += 20; // Increase hunger (you can adjust the amount)
        if (avatar_data.hunger > 100)
            avatar_data.hunger = 100;
        ESP_LOGI(TAG, "Avatar fed. Hunger: %d", avatar_data.hunger);
        update_avatar_gif();             // Update GIF based on new state
        avatar_state_save(&avatar_data); // Save the updated state
    }
    else
    {
        ESP_LOGI(TAG, "Avatar is already full.");
        // Optionally play a "full" animation
    }
}

// Event handler for the "Play" button
void btn_play_cb(lv_event_t *e)
{
    LV_UNUSED(e);
    track_action("Play");
    if (avatar_data.boredom > 0)
    {
        avatar_data.boredom -= 30; // Decrease boredom
        if (avatar_data.boredom < 0)
            avatar_data.boredom = 0;
        ESP_LOGI(TAG, "Played with avatar. Boredom: %d", avatar_data.boredom);
        update_avatar_gif(); // Update GIF
        avatar_state_save(&avatar_data);
    }
    else
    {
        ESP_LOGI(TAG, "Avatar is not bored.");
        // Optionally play a "not bored" animation
    }
}

// Function to set the avatar's GIF based on its state
void update_avatar_gif(void)
{
    if (avatar_data.is_sleeping)
    {
        // Set sleeping GIF (you'll need a gif_sleep.c)
        // lv_img_set_src(img_avatar, &gif_sleep);
        ESP_LOGI(TAG, "Avatar is sleeping (GIF not yet implemented).");
    }
    else if (avatar_data.hunger > 80 && avatar_data.boredom < 20)
    {
        // Example: Happy animation
        lv_img_set_src(img_avatar, &gif_rabbit);
    }
    else
    {
        // Default or idle animation
        lv_img_set_src(img_avatar, &gif_cat);
    }
}

// Function to track actions
void track_action(const char *action_name)
{
    ESP_LOGI(TAG, "Action: %s", action_name);
    // You might want to store these actions or trigger other events
}