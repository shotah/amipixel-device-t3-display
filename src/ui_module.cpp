#include "ui_module.h"
#include "avatar_state.h"
#include "globals.h"
#include <LilyGo_AMOLED.h>
#include <lvgl.h>

// Remove the complex ESP32 includes for now - just use basic debugging
#include "data/gifs/gif_idle.h"
#include "data/gifs/gif_happy.h"
#include "data/gifs/gif_sleeping.h"
#include "data/gifs/gif_eating.h"
// Removed: #include "esp_log.h"  // Not compatible with Windows development

// Removed TAG since we're using Serial.println now

// Global instance of the avatar state
avatar_state_t avatar_data;

// Screen
lv_obj_t *screen_main;

// Buttons
lv_obj_t *btn_feed;
lv_obj_t *btn_play;
lv_obj_t *btn_pet;

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
        avatar_state_save(&avatar_data);
    }
    else
    {
        Serial.println("Avatar state loaded successfully.");
    }

    // Create the main screen
    screen_main = lv_obj_create(NULL);
    
    // Initialize main screen
    create_main_screen();
    
    // Load main screen
    lv_scr_load(screen_main);
}

// Function to create the main screen
void create_main_screen(void)
{
    // Set a light background for the entire screen
    lv_obj_set_style_bg_color(screen_main, lv_color_hex(0xF0F0F0), 0);  // Light gray background
    
    // Create the avatar image object - properly sized for 60x40 GIF data
    img_avatar = lv_img_create(screen_main);
    lv_obj_set_size(img_avatar, 120, 80);  // 2x scale of 60x40 for better visibility
    lv_obj_align(img_avatar, LV_ALIGN_CENTER, 0, -40);  // Slightly above center
    
    // Remove all background styling - let the image show through transparently
    lv_obj_set_style_bg_opa(img_avatar, LV_OPA_TRANSP, 0);  // Transparent background
    lv_obj_set_style_border_width(img_avatar, 0, 0);  // No border
    
    // Enable image scaling and set proper image opacity
    lv_obj_set_style_img_opa(img_avatar, LV_OPA_COVER, 0);
    
    Serial.println("Avatar object created, setting initial image...");
    
    // Debug: Check object positioning and parent
    lv_coord_t x = lv_obj_get_x(img_avatar);
    lv_coord_t y = lv_obj_get_y(img_avatar);
    lv_coord_t w = lv_obj_get_width(img_avatar);
    lv_coord_t h = lv_obj_get_height(img_avatar);
    Serial.printf("Avatar object position: x=%d, y=%d, size=%dx%d\n", x, y, w, h);
    Serial.printf("Avatar object parent: %p, screen: %p\n", lv_obj_get_parent(img_avatar), screen_main);
    
    update_avatar_gif(); // Set initial image based on loaded state
    Serial.println("Avatar image set successfully.");

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
        Serial.printf("Avatar fed. Hunger: %d\n", avatar_data.hunger);
        update_avatar_gif();             // Update GIF based on new state
        avatar_state_save(&avatar_data); // Save the updated state
    }
    else
    {
        Serial.println("Avatar is already full.");
        // Optionally play a "full" animation
    }
}

// Event handler for the "Play" button
void btn_play_cb(lv_event_t *e)
{
    LV_UNUSED(e);
    track_action("Play");
    
    if (avatar_data.boredom > 20)
    {
        avatar_data.boredom -= 20;
        if (avatar_data.boredom < 0)
            avatar_data.boredom = 0;
        Serial.printf("Played with avatar. Boredom: %d\n", avatar_data.boredom);
        update_avatar_gif(); // Update GIF
        avatar_state_save(&avatar_data);
    }
    else
    {
        Serial.println("Avatar is not bored.");
        // Optionally play a "not bored" animation
    }
}

// Event handler for the "Pet" button
void btn_pet_cb(lv_event_t *e)
{
    LV_UNUSED(e);
    track_action("Pet");
    
    if (avatar_data.happiness < 80)
    {
        avatar_data.happiness += 20;
        if (avatar_data.happiness > 100)
            avatar_data.happiness = 100;
        Serial.printf("Avatar petted. Happiness: %d\n", avatar_data.happiness);
        update_avatar_gif(); // Update GIF
        avatar_state_save(&avatar_data);
    }
    else
    {
        Serial.println("Avatar is already very happy!");
        // Optionally play a "very happy" animation
    }
}

// Event handler for the "Sleep" button
void btn_sleep_cb(lv_event_t *e)
{
    LV_UNUSED(e);
    track_action("Sleep");
    avatar_data.is_sleeping = !avatar_data.is_sleeping;
    
    if (avatar_data.is_sleeping)
    {
        Serial.println("Avatar is now sleeping. Zzz...");
        // Sleeping restores a bit of everything over time
        avatar_data.happiness += 5;
        if (avatar_data.happiness > 100)
            avatar_data.happiness = 100;
        avatar_data.hunger -= 5;
        if (avatar_data.hunger < 0)
            avatar_data.hunger = 0;
        update_avatar_gif(); // Update to sleeping GIF
        avatar_state_save(&avatar_data);
    }
    else
    {
        Serial.println("Avatar woke up!");
        // Waking up might make them a bit hungry
        avatar_data.hunger -= 10;
        if (avatar_data.hunger < 0)
            avatar_data.hunger = 0;
        update_avatar_gif(); // Update back to normal state
        avatar_state_save(&avatar_data);
    }
}



// Function to set the avatar's GIF based on its state
void update_avatar_gif(void)
{
    // NULL check for safety
    if (img_avatar == NULL) 
    {
        Serial.println("ERROR: Avatar update skipped - img_avatar is NULL");
        return;
    }
    
    // Check if the object is valid and visible
    Serial.printf("Avatar object info - Size: %dx%d, Visible: %s\n", 
                 lv_obj_get_width(img_avatar), lv_obj_get_height(img_avatar),
                 lv_obj_has_flag(img_avatar, LV_OBJ_FLAG_HIDDEN) ? "NO" : "YES");
    
    Serial.printf("Updating avatar image - Sleep: %d, Hunger: %d, Boredom: %d, Happiness: %d\n", 
             avatar_data.is_sleeping, avatar_data.hunger, avatar_data.boredom, avatar_data.happiness);
             
    if (avatar_data.is_sleeping)
    {
        // Set sleeping image
        Serial.println("Setting sleeping image (&gif_sleeping)");
        Serial.printf("GIF data info - w:%d, h:%d, size:%d\n", gif_sleeping.header.w, gif_sleeping.header.h, gif_sleeping.data_size);
        lv_img_set_src(img_avatar, &gif_sleeping);
        Serial.println("✓ gif_sleeping set");
    }
    else if (avatar_data.hunger < 20)
    {
        // Hungry - use eating animation
        Serial.println("Setting eating image (&gif_eating)");
        Serial.printf("GIF data info - w:%d, h:%d, size:%d\n", gif_eating.header.w, gif_eating.header.h, gif_eating.data_size);
        lv_img_set_src(img_avatar, &gif_eating);
        Serial.println("✓ gif_eating set");
    }
    else if (avatar_data.hunger > 80 && avatar_data.boredom < 20 && avatar_data.happiness > 80)
    {
        // Happy animation
        Serial.println("Setting happy image (&gif_happy)");
        Serial.printf("GIF data info - w:%d, h:%d, size:%d\n", gif_happy.header.w, gif_happy.header.h, gif_happy.data_size);
        lv_img_set_src(img_avatar, &gif_happy);
        Serial.println("✓ gif_happy set");
    }
    else
    {
        // Default or idle animation
        Serial.println("Setting idle image (&gif_idle)");
        Serial.printf("GIF data info - w:%d, h:%d, size:%d\n", gif_idle.header.w, gif_idle.header.h, gif_idle.data_size);
        
        // Debug: Check first few bytes of GIF data
        const uint8_t* data = (const uint8_t*)gif_idle.data;
        Serial.printf("First 16 bytes: ");
        for(int i = 0; i < 16; i++) {
            Serial.printf("0x%02x ", data[i]);
        }
        Serial.println();
        
        lv_img_set_src(img_avatar, &gif_idle);
        Serial.println("✓ gif_idle set");
    }
    
    // Force refresh ONLY if we made it this far safely
    lv_obj_invalidate(img_avatar);
    
    // Additional debug: check image source after setting
    const void* src = lv_img_get_src(img_avatar);
    Serial.printf("Image source after setting: %p\n", src);
    
    Serial.println("Avatar image update completed safely");
}

// Test function to create a simple colored rectangle for debugging
void test_avatar_display(void) 
{
    Serial.println("Testing avatar display with black/white pattern...");
    
    if (img_avatar == NULL) {
        Serial.println("ERROR: img_avatar is NULL in test function");
        return;
    }
    
    // Create a simple 60x40 black/white pattern (like our GIF data)
    static uint8_t test_data[60 * 40 * 2]; // RGB565, 2 bytes per pixel
    
    // Create black/white checkerboard using same format as working test
    for (int y = 0; y < 40; y++) {
        for (int x = 0; x < 60; x++) {
            int index = (y * 60 + x) * 2;
            
            // 8x8 checkerboard pattern
            bool is_black = ((x / 8) + (y / 8)) % 2 == 0;
            
            if (is_black) {
                // Black: RGB565 = 0x0000 (same as our GIF)
                test_data[index] = 0x00;     // Low byte
                test_data[index + 1] = 0x00; // High byte
            } else {
                // White: RGB565 = 0xFFFF (same as our GIF)
                test_data[index] = 0xFF;     // Low byte
                test_data[index + 1] = 0xFF; // High byte
            }
        }
    }
    
    // Create image descriptor
    static lv_img_dsc_t test_img = {
        .header = {
            .cf = LV_COLOR_FORMAT_NATIVE,
            .w = 60,
            .h = 40
        },
        .data_size = sizeof(test_data),
        .data = test_data
    };
    
    Serial.println("Setting test black/white pattern as avatar image...");
    lv_img_set_src(img_avatar, &test_img);
    lv_obj_invalidate(img_avatar);
    Serial.println("✓ Test black/white pattern set - should see black/white squares");
}

// Function to track actions
void track_action(const char *action_name)
{
    Serial.printf("Action: %s\n", action_name);
    // You might want to store these actions or trigger other events
}

