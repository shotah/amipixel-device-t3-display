#include "avatar_state.h"
#include <Arduino.h>     // For Serial.printf/println
// Removed: #include "esp_log.h"  // Not compatible with Windows development
#include "nvs_flash.h"
#include "nvs.h"
#include <string.h>    // For string manipulation

// Removed TAG since we're using Serial.println now
// Removed unused constants - using hardcoded strings now

// Function to initialize the avatar's state
void avatar_state_init(avatar_state_t *state)
{
    if (state == NULL)
        return;

    state->hunger = 50;
    state->boredom = 30;
    state->happiness = 70;
    state->is_sleeping = false;
    strcpy(state->name, "Pet");
    
    Serial.println("[AVATAR] Avatar state initialized with defaults.");
}

// Function to save the avatar's state to NVS
void avatar_state_save(avatar_state_t *state)
{
    if (state == NULL)
        return;

    nvs_handle_t nvs_handle;
    esp_err_t err;

    err = nvs_open("avatar", NVS_READWRITE, &nvs_handle);
    if (err != ESP_OK)
    {
        Serial.printf("[AVATAR] Error opening NVS: %s\n", esp_err_to_name(err));
        return;
    }

    err = nvs_set_i32(nvs_handle, "hunger", state->hunger);
    if (err != ESP_OK)
    {
        Serial.printf("[AVATAR] Error saving hunger to NVS: %s\n", esp_err_to_name(err));
        nvs_close(nvs_handle);
        return;
    }

    err = nvs_set_i32(nvs_handle, "boredom", state->boredom);
    if (err != ESP_OK)
    {
        Serial.printf("[AVATAR] Error saving boredom to NVS: %s\n", esp_err_to_name(err));
        nvs_close(nvs_handle);
        return;
    }

    err = nvs_set_i32(nvs_handle, "happiness", state->happiness);
    if (err != ESP_OK)
    {
        Serial.printf("[AVATAR] Error saving happiness to NVS: %s\n", esp_err_to_name(err));
        nvs_close(nvs_handle);
        return;
    }

    err = nvs_set_u8(nvs_handle, "sleeping", state->is_sleeping ? 1 : 0);
    if (err != ESP_OK)
    {
        Serial.printf("[AVATAR] Error saving sleeping state to NVS: %s\n", esp_err_to_name(err));
        nvs_close(nvs_handle);
        return;
    }

    err = nvs_set_str(nvs_handle, "name", state->name);
    if (err != ESP_OK)
    {
        Serial.printf("[AVATAR] Error saving name to NVS: %s\n", esp_err_to_name(err));
        nvs_close(nvs_handle);
        return;
    }

    err = nvs_commit(nvs_handle);
    if (err != ESP_OK)
    {
        Serial.printf("[AVATAR] Error committing NVS: %s\n", esp_err_to_name(err));
        nvs_close(nvs_handle);
        return;
    }

    nvs_close(nvs_handle);
    Serial.println("[AVATAR] Avatar state saved to NVS.");
}

// Function to load the avatar's state from NVS
bool avatar_state_load(avatar_state_t *state)
{
    if (state == NULL)
        return false;

    nvs_handle_t nvs_handle;
    esp_err_t err;

    err = nvs_open("avatar", NVS_READONLY, &nvs_handle);
    if (err != ESP_OK)
    {
        Serial.printf("[AVATAR] Error opening NVS: %s\n", esp_err_to_name(err));
        return false;
    }

    err = nvs_get_i32(nvs_handle, "hunger", &state->hunger);
    if (err != ESP_OK)
    {
        Serial.printf("[AVATAR] Error loading hunger from NVS: %s\n", esp_err_to_name(err));
    }

    err = nvs_get_i32(nvs_handle, "boredom", &state->boredom);
    if (err != ESP_OK)
    {
        Serial.printf("[AVATAR] Error loading boredom from NVS: %s\n", esp_err_to_name(err));
    }

    err = nvs_get_i32(nvs_handle, "happiness", &state->happiness);
    if (err != ESP_OK)
    {
        Serial.printf("[AVATAR] Error loading happiness from NVS: %s\n", esp_err_to_name(err));
    }

    uint8_t sleeping;
    err = nvs_get_u8(nvs_handle, "sleeping", &sleeping);
    if (err != ESP_OK)
    {
        Serial.printf("[AVATAR] Error loading sleeping state from NVS: %s\n", esp_err_to_name(err));
    }
    else
    {
        state->is_sleeping = (sleeping == 1);
    }

    // Load name with proper buffer handling
    size_t required_size = sizeof(state->name);
    err = nvs_get_str(nvs_handle, "name", state->name, &required_size);
    if (err == ESP_ERR_NVS_NOT_FOUND)
    {
        strcpy(state->name, "Pet");
    }
    else if (err != ESP_OK)
    {
        Serial.printf("[AVATAR] Error loading name from NVS: %s\n", esp_err_to_name(err));
        strcpy(state->name, "Pet");
    }
    else if (required_size > sizeof(state->name))
    {
        Serial.println("[AVATAR] Name in NVS is too long, using default.");
        strcpy(state->name, "Pet");
    }
    else
    {
        err = nvs_get_str(nvs_handle, "name", state->name, &required_size);
        if (err != ESP_OK)
        {
            Serial.printf("[AVATAR] Error getting name length from NVS: %s\n", esp_err_to_name(err));
            strcpy(state->name, "Pet");
        }
    }

    nvs_close(nvs_handle);
    Serial.println("[AVATAR] Avatar state loaded from NVS.");
    return true;
}