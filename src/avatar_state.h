#ifndef AVATAR_STATE_H
#define AVATAR_STATE_H

// Include necessary headers
#include <stdint.h>
#include <stdbool.h>

// Define the structure to hold the avatar's state
typedef struct
{
    uint8_t hunger;    // 0 to 100 (e.g., 100 is full)
    uint8_t boredom;   // 0 to 100 (e.g., 100 is not bored)
    uint8_t happiness; // 0 to 100
    // Add more stats as needed (e.g., cleanliness, energy, level, etc.)
    uint8_t is_sleeping; // 0 for false, 1 for true
    // Potentially add a name for the avatar
    char name[32]; // Example: Up to 31 characters + null terminator
} avatar_state_t;

// Function to initialize the avatar's state (sets default starting values)
void avatar_state_init(avatar_state_t *state);

// Function to save the avatar's state to persistent storage
bool avatar_state_save(const avatar_state_t *state);

// Function to load the avatar's state from persistent storage
bool avatar_state_load(avatar_state_t *state);

#endif // AVATAR_STATE_H