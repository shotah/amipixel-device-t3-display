#include "ui_module.h"
#include "data/gifs/gif_idle.h"
#include "data/gifs/gif_happy.h"
#include "data/gifs/gif_sleeping.h"
#include "data/gifs/gif_eating.h"
#include "esp_log.h"

static const char *TAG = "UI_MODULE";

// Global instance of the avatar state
avatar_state_t avatar_data;

// Screen
lv_obj_t *screen_main;
lv_obj_t *screen_stats;

// Buttons
lv_obj_t *btn_feed;
lv_obj_t *btn_play;
lv_obj_t *btn_pet;
lv_obj_t *btn_stats;
lv_obj_t *btn_back;

// Image to display the avatar GIF
lv_obj_t *img_avatar;

// Health stats display (only on stats screen)
lv_obj_t *hunger_bar;
lv_obj_t *boredom_bar;
lv_obj_t *happiness_bar;
lv_obj_t *hunger_label;
lv_obj_t *boredom_label;
lv_obj_t *happiness_label;

// Current screen state
typedef enum {
    SCREEN_MAIN,
    SCREEN_STATS
} screen_state_t;

screen_state_t current_screen = SCREEN_MAIN;

// Function to switch between screens
void switch_to_main_screen(void);
void switch_to_stats_screen(void);

// Function to initialize the UI
void ui_init(void)
{
    lv_init(); // Make sure LVGL is initialized elsewhere

    // Load avatar state from NVS
    if (!avatar_state_load(&avatar_data))
    {
        avatar_state_init(&avatar_data);
        avatar_state_save(&avatar_data);
    }
    else
    {
        ESP_LOGI(TAG, "Avatar state loaded successfully.");
    }

    // Create the main screen
    screen_main = lv_obj_create(NULL);
    
    // Create the stats screen
    screen_stats = lv_obj_create(NULL);
    
    // Initialize both screens
    create_main_screen();
    create_stats_screen();
    
    // Load main screen by default
    switch_to_main_screen();
}

// Function to create the main screen
void create_main_screen(void)
{
    // Set a light background for the entire screen
    lv_obj_set_style_bg_color(screen_main, lv_color_hex(0xF0F0F0), 0);  // Light gray background
    
    // Create the avatar image object - centered and prominent
    img_avatar = lv_img_create(screen_main);
    lv_obj_set_size(img_avatar, 180, 120);  // Even bigger for visibility
    lv_obj_align(img_avatar, LV_ALIGN_CENTER, 0, -40);  // Slightly above center
    
    // White background for avatar area (so black stick figure shows up)
    lv_obj_set_style_bg_color(img_avatar, lv_color_white(), 0);
    lv_obj_set_style_border_width(img_avatar, 2, 0);
    lv_obj_set_style_border_color(img_avatar, lv_color_hex(0x4F5D75), 0);
    lv_obj_set_style_radius(img_avatar, 12, 0);
    
    ESP_LOGI(TAG, "Avatar object created, setting initial image...");
    update_avatar_gif(); // Set initial image based on loaded state
    ESP_LOGI(TAG, "Avatar image set successfully.");

    // Create title label
    lv_obj_t *title_label = lv_label_create(screen_main);
    lv_label_set_text(title_label, "AmiPixel Pet");
    lv_obj_set_style_text_color(title_label, lv_color_hex(0x333333), 0);
    lv_obj_set_style_text_font(title_label, &lv_font_montserrat_18, 0);
    lv_obj_align(title_label, LV_ALIGN_TOP_MID, 0, 20);

    // CORNER BUTTONS LAYOUT
    // Top-Left: Feed Button
    btn_feed = lv_btn_create(screen_main);
    lv_obj_set_size(btn_feed, 80, 60);
    lv_obj_align(btn_feed, LV_ALIGN_TOP_LEFT, 10, 10);
    lv_obj_add_event_cb(btn_feed, btn_feed_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_set_style_bg_color(btn_feed, lv_color_hex(0xFF6B6B), 0);
    lv_obj_set_style_radius(btn_feed, 12, 0);
    lv_obj_t *label_feed = lv_label_create(btn_feed);
    lv_label_set_text(label_feed, LV_SYMBOL_PLUS "\nFeed");
    lv_obj_set_style_text_align(label_feed, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_style_text_color(label_feed, lv_color_white(), 0);
    lv_obj_center(label_feed);

    // Top-Right: Play Button
    btn_play = lv_btn_create(screen_main);
    lv_obj_set_size(btn_play, 80, 60);
    lv_obj_align(btn_play, LV_ALIGN_TOP_RIGHT, -10, 10);
    lv_obj_add_event_cb(btn_play, btn_play_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_set_style_bg_color(btn_play, lv_color_hex(0x4CAF50), 0);
    lv_obj_set_style_radius(btn_play, 12, 0);
    lv_obj_t *label_play = lv_label_create(btn_play);
    lv_label_set_text(label_play, LV_SYMBOL_PLAY "\nPlay");
    lv_obj_set_style_text_align(label_play, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_style_text_color(label_play, lv_color_white(), 0);
    lv_obj_center(label_play);

    // Bottom-Left: Pet Button
    btn_pet = lv_btn_create(screen_main);
    lv_obj_set_size(btn_pet, 80, 60);
    lv_obj_align(btn_pet, LV_ALIGN_BOTTOM_LEFT, 10, -10);
    lv_obj_add_event_cb(btn_pet, btn_pet_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_set_style_bg_color(btn_pet, lv_color_hex(0xFFB74D), 0);
    lv_obj_set_style_radius(btn_pet, 12, 0);
    lv_obj_t *label_pet = lv_label_create(btn_pet);
    lv_label_set_text(label_pet, LV_SYMBOL_AUDIO "\nPet");
    lv_obj_set_style_text_align(label_pet, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_style_text_color(label_pet, lv_color_white(), 0);
    lv_obj_center(label_pet);

    // Bottom-Right: Sleep Button (new 4th corner action)
    lv_obj_t *btn_sleep = lv_btn_create(screen_main);
    lv_obj_set_size(btn_sleep, 80, 60);
    lv_obj_align(btn_sleep, LV_ALIGN_BOTTOM_RIGHT, -10, -10);
    lv_obj_add_event_cb(btn_sleep, btn_sleep_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_set_style_bg_color(btn_sleep, lv_color_hex(0x9C27B0), 0);
    lv_obj_set_style_radius(btn_sleep, 12, 0);
    lv_obj_t *label_sleep = lv_label_create(btn_sleep);
    lv_label_set_text(label_sleep, LV_SYMBOL_PAUSE "\nSleep");
    lv_obj_set_style_text_align(label_sleep, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_style_text_color(label_sleep, lv_color_white(), 0);
    lv_obj_center(label_sleep);

    // CENTER: Stats Button
    btn_stats = lv_btn_create(screen_main);
    lv_obj_set_size(btn_stats, 90, 40);
    lv_obj_align(btn_stats, LV_ALIGN_CENTER, 0, 80);  // Below the avatar
    lv_obj_add_event_cb(btn_stats, btn_stats_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_set_style_bg_color(btn_stats, lv_color_hex(0x2D3142), 0);
    lv_obj_set_style_radius(btn_stats, 10, 0);
    lv_obj_t *label_stats = lv_label_create(btn_stats);
    lv_label_set_text(label_stats, LV_SYMBOL_LIST " Stats");
    lv_obj_set_style_text_color(label_stats, lv_color_white(), 0);
    lv_obj_center(label_stats);
}

// Function to create the stats screen
void create_stats_screen(void)
{
    // Create health stats display on stats screen
    create_health_stats_display(screen_stats);

    // Create "Back" button
    btn_back = lv_btn_create(screen_stats);
    lv_obj_set_size(btn_back, 100, 30);
    lv_obj_align(btn_back, LV_ALIGN_BOTTOM_MID, 0, -10);
    lv_obj_add_event_cb(btn_back, btn_back_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_set_style_bg_color(btn_back, lv_color_hex(0x2D3142), 0);
    lv_obj_t *label_back = lv_label_create(btn_back);
    lv_label_set_text(label_back, LV_SYMBOL_LEFT " Back");
    lv_obj_center(label_back);
}

// Function to switch to main screen
void switch_to_main_screen(void)
{
    lv_scr_load(screen_main);
    current_screen = SCREEN_MAIN;
    ESP_LOGI(TAG, "Switched to main screen");
}

// Function to switch to stats screen
void switch_to_stats_screen(void)
{
    lv_scr_load(screen_stats);
    current_screen = SCREEN_STATS;
    update_health_stats_display(); // Update stats when showing
    ESP_LOGI(TAG, "Switched to stats screen");
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

// Event handler for the "Pet" button
void btn_pet_cb(lv_event_t *e)
{
    LV_UNUSED(e);
    track_action("Pet");
    if (avatar_data.happiness < 100)
    {
        avatar_data.happiness += 15; // Increase happiness
        if (avatar_data.happiness > 100)
            avatar_data.happiness = 100;
        ESP_LOGI(TAG, "Avatar petted. Happiness: %d", avatar_data.happiness);
        update_avatar_gif(); // Update GIF
        avatar_state_save(&avatar_data);
    }
    else
    {
        ESP_LOGI(TAG, "Avatar is already very happy!");
        // Optionally play a "very happy" animation
    }
}

// Event handler for the "Sleep" button
void btn_sleep_cb(lv_event_t *e)
{
    LV_UNUSED(e);
    track_action("Sleep");
    
    // Toggle sleep state
    avatar_data.is_sleeping = !avatar_data.is_sleeping;
    
    if (avatar_data.is_sleeping)
    {
        ESP_LOGI(TAG, "Avatar is now sleeping. Zzz...");
        // Sleeping restores a bit of everything over time
        avatar_data.happiness += 5;
        if (avatar_data.happiness > 100) avatar_data.happiness = 100;
    }
    else
    {
        ESP_LOGI(TAG, "Avatar woke up!");
        // Waking up might make them a bit hungry
        avatar_data.hunger -= 10;
        if (avatar_data.hunger < 0) avatar_data.hunger = 0;
    }
    
    update_avatar_gif(); // Update GIF to show sleep state
    avatar_state_save(&avatar_data);
}

// Event handler for the "Stats" button
void btn_stats_cb(lv_event_t *e)
{
    LV_UNUSED(e);
    switch_to_stats_screen();
}

// Event handler for the "Back" button
void btn_back_cb(lv_event_t *e)
{
    LV_UNUSED(e);
    switch_to_main_screen();
}

// Function to set the avatar's image based on its state
void update_avatar_gif(void)
{
    ESP_LOGI(TAG, "Updating avatar image - Sleep: %d, Hunger: %d, Boredom: %d, Happiness: %d", 
             avatar_data.is_sleeping, avatar_data.hunger, avatar_data.boredom, avatar_data.happiness);
             
    if (avatar_data.is_sleeping)
    {
        // Set sleeping image
        ESP_LOGI(TAG, "Setting sleeping image");
        lv_img_set_src(img_avatar, &gif_sleeping);
    }
    else if (avatar_data.hunger < 20)
    {
        // Hungry - use eating animation
        ESP_LOGI(TAG, "Setting eating image (hungry)");
        lv_img_set_src(img_avatar, &gif_eating);
    }
    else if (avatar_data.hunger > 80 && avatar_data.boredom < 20 && avatar_data.happiness > 80)
    {
        // Happy animation
        ESP_LOGI(TAG, "Setting happy image");
        lv_img_set_src(img_avatar, &gif_happy);
    }
    else
    {
        // Default or idle animation
        ESP_LOGI(TAG, "Setting idle image");
        lv_img_set_src(img_avatar, &gif_idle);
    }
    
    // Force refresh
    lv_obj_invalidate(img_avatar);
}

// Function to track actions
void track_action(const char *action_name)
{
    ESP_LOGI(TAG, "Action: %s", action_name);
    // You might want to store these actions or trigger other events
}

// Function to create health stats display
void create_health_stats_display(lv_obj_t *parent)
{
    // Create container for health stats
    lv_obj_t *stats_container = lv_obj_create(parent);
    lv_obj_set_size(stats_container, LV_PCT(90), 200);
    lv_obj_align(stats_container, LV_ALIGN_CENTER, 0, -20);
    lv_obj_set_style_bg_color(stats_container, lv_color_hex(0x2D3142), 0);
    lv_obj_set_style_border_color(stats_container, lv_color_hex(0x4F5D75), 0);
    lv_obj_set_style_border_width(stats_container, 2, 0);
    lv_obj_set_style_radius(stats_container, 10, 0);

    // Title
    lv_obj_t *title = lv_label_create(stats_container);
    lv_label_set_text(title, LV_SYMBOL_LIST " Avatar Stats");
    lv_obj_set_style_text_color(title, lv_color_white(), 0);
    lv_obj_set_style_text_font(title, &lv_font_montserrat_18, 0);
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 10);

    // Hunger stats
    hunger_label = lv_label_create(stats_container);
    lv_label_set_text(hunger_label, LV_SYMBOL_PLUS " Hunger");
    lv_obj_set_style_text_color(hunger_label, lv_color_white(), 0);
    lv_obj_align(hunger_label, LV_ALIGN_TOP_LEFT, 20, 50);

    hunger_bar = lv_bar_create(stats_container);
    lv_obj_set_size(hunger_bar, 150, 12);
    lv_obj_align_to(hunger_bar, hunger_label, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 10);
    lv_bar_set_range(hunger_bar, 0, 100);
    lv_obj_set_style_bg_color(hunger_bar, lv_color_hex(0x666666), LV_PART_MAIN);
    lv_obj_set_style_bg_color(hunger_bar, lv_color_hex(0xFF6B6B), LV_PART_INDICATOR);

    // Boredom stats
    boredom_label = lv_label_create(stats_container);
    lv_label_set_text(boredom_label, LV_SYMBOL_PLAY " Boredom");
    lv_obj_set_style_text_color(boredom_label, lv_color_white(), 0);
    lv_obj_align_to(boredom_label, hunger_bar, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 20);

    boredom_bar = lv_bar_create(stats_container);
    lv_obj_set_size(boredom_bar, 150, 12);
    lv_obj_align_to(boredom_bar, boredom_label, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 10);
    lv_bar_set_range(boredom_bar, 0, 100);
    lv_obj_set_style_bg_color(boredom_bar, lv_color_hex(0x666666), LV_PART_MAIN);
    lv_obj_set_style_bg_color(boredom_bar, lv_color_hex(0xFFB74D), LV_PART_INDICATOR);

    // Happiness stats
    happiness_label = lv_label_create(stats_container);
    lv_label_set_text(happiness_label, LV_SYMBOL_AUDIO " Happiness");
    lv_obj_set_style_text_color(happiness_label, lv_color_white(), 0);
    lv_obj_align_to(happiness_label, boredom_bar, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 20);

    happiness_bar = lv_bar_create(stats_container);
    lv_obj_set_size(happiness_bar, 150, 12);
    lv_obj_align_to(happiness_bar, happiness_label, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 10);
    lv_bar_set_range(happiness_bar, 0, 100);
    lv_obj_set_style_bg_color(happiness_bar, lv_color_hex(0x666666), LV_PART_MAIN);
    lv_obj_set_style_bg_color(happiness_bar, lv_color_hex(0x4CAF50), LV_PART_INDICATOR);

    // Initial update
    update_health_stats_display();
}

// Function to update health stats display
void update_health_stats_display(void)
{
    // Only update if we're on the stats screen
    if (current_screen == SCREEN_STATS) {
        // Update hunger bar
        lv_bar_set_value(hunger_bar, avatar_data.hunger, LV_ANIM_ON);
        
        // Update boredom bar
        lv_bar_set_value(boredom_bar, avatar_data.boredom, LV_ANIM_ON);
        
        // Update happiness bar
        lv_bar_set_value(happiness_bar, avatar_data.happiness, LV_ANIM_ON);
        
        ESP_LOGI(TAG, "Health stats updated - Hunger: %d, Boredom: %d, Happiness: %d", 
                 avatar_data.hunger, avatar_data.boredom, avatar_data.happiness);
    }
}