#include "avatar_state.h"
#include "nvs_flash.h" // For ESP32 Non-Volatile Storage (NVS)
#include "esp_log.h"   // For logging (optional)
#include <string.h>    // For string manipulation

static const char *TAG = "AVATAR_STATE"; // Tag for ESP-IDF logging
static const char *NAMESPACE = "avatar_data";
static const char *KEY_HUNGER = "hunger";
static const char *KEY_BOREDOM = "boredom";
static const char *KEY_HAPPINESS = "happiness";
static const char *KEY_SLEEPING = "sleeping";
static const char *KEY_NAME = "name";

// Function to initialize the avatar's state
void avatar_state_init(avatar_state_t *state)
{
    state->hunger = 100;   // Initial full value
    state->boredom = 50;   // Initial medium boredom
    state->happiness = 75; // Initial good mood
    state->is_sleeping = false;
    strcpy(state->name, "Ami"); // Default name
    ESP_LOGI(TAG, "Avatar state initialized with defaults.");
}

// Function to save the avatar's state to NVS
bool avatar_state_save(const avatar_state_t *state)
{
    esp_err_t err;
    nvs_handle_t nvs_handle;

    err = nvs_open(NAMESPACE, NVS_READWRITE, &nvs_handle);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Error opening NVS: %s", esp_err_to_name(err));
        return false;
    }

    err = nvs_set_u8(nvs_handle, KEY_HUNGER, state->hunger);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Error saving hunger to NVS: %s", esp_err_to_name(err));
        nvs_close(nvs_handle);
        return false;
    }

    err = nvs_set_u8(nvs_handle, KEY_BOREDOM, state->boredom);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Error saving boredom to NVS: %s", esp_err_to_name(err));
        nvs_close(nvs_handle);
        return false;
    }

    err = nvs_set_u8(nvs_handle, KEY_HAPPINESS, state->happiness);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Error saving happiness to NVS: %s", esp_err_to_name(err));
        nvs_close(nvs_handle);
        return false;
    }

    err = nvs_set_u8(nvs_handle, KEY_SLEEPING, state->is_sleeping);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Error saving sleeping state to NVS: %s", esp_err_to_name(err));
        nvs_close(nvs_handle);
        return false;
    }

    err = nvs_set_str(nvs_handle, KEY_NAME, state->name);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Error saving name to NVS: %s", esp_err_to_name(err));
        nvs_close(nvs_handle);
        return false;
    }

    err = nvs_commit(nvs_handle);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Error committing NVS: %s", esp_err_to_name(err));
        nvs_close(nvs_handle);
        return false;
    }

    nvs_close(nvs_handle);
    ESP_LOGI(TAG, "Avatar state saved to NVS.");
    return true;
}

// Function to load the avatar's state from NVS
bool avatar_state_load(avatar_state_t *state)
{
    esp_err_t err;
    nvs_handle_t nvs_handle;

    err = nvs_open(NAMESPACE, NVS_READONLY, &nvs_handle);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Error opening NVS: %s", esp_err_to_name(err));
        return false;
    }

    err = nvs_get_u8(nvs_handle, KEY_HUNGER, &state->hunger);
    if (err != ESP_OK && err != ESP_ERR_NVS_NO_FREE_PAGES && err != ESP_ERR_NVS_NOT_FOUND)
        ESP_LOGE(TAG, "Error loading hunger from NVS: %s", esp_err_to_name(err));

    err = nvs_get_u8(nvs_handle, KEY_BOREDOM, &state->boredom);
    if (err != ESP_OK && err != ESP_ERR_NVS_NO_FREE_PAGES && err != ESP_ERR_NVS_NOT_FOUND)
        ESP_LOGE(TAG, "Error loading boredom from NVS: %s", esp_err_to_name(err));

    err = nvs_get_u8(nvs_handle, KEY_HAPPINESS, &state->happiness);
    if (err != ESP_OK && err != ESP_ERR_NVS_NO_FREE_PAGES && err != ESP_ERR_NVS_NOT_FOUND)
        ESP_LOGE(TAG, "Error loading happiness from NVS: %s", esp_err_to_name(err));

    err = nvs_get_u8(nvs_handle, KEY_SLEEPING, &state->is_sleeping);
    if (err != ESP_OK && err != ESP_ERR_NVS_NO_FREE_PAGES && err != ESP_ERR_NVS_NOT_FOUND)
        ESP_LOGE(TAG, "Error loading sleeping state from NVS: %s", esp_err_to_name(err));

    size_t name_len;
    err = nvs_get_str(nvs_handle, KEY_NAME, NULL, &name_len); // Get required size
    if (err == ESP_OK)
    {
        if (name_len < sizeof(state->name))
        {
            err = nvs_get_str(nvs_handle, KEY_NAME, state->name, &name_len);
            if (err != ESP_OK)
                ESP_LOGE(TAG, "Error loading name from NVS: %s", esp_err_to_name(err));
        }
        else
        {
            ESP_LOGW(TAG, "Name in NVS is too long, using default.");
            strcpy(state->name, "Ami");
        }
    }
    else if (err != ESP_ERR_NVS_NO_FREE_PAGES && err != ESP_ERR_NVS_NOT_FOUND)
    {
        ESP_LOGE(TAG, "Error getting name length from NVS: %s", esp_err_to_name(err));
    }

    nvs_close(nvs_handle);
    ESP_LOGI(TAG, "Avatar state loaded from NVS.");
    return true;
}