// actions.cpp
#include "actions.h"
#include <iostream> // For debug output (optional, can use Serial.println later)

void do_action_feed() {
    std::cout << "Action: Feed the AmiPixel!" << std::endl; // Debug message
    // In the future, we'll add logic to affect the avatar's hunger need here
}

void do_action_play() {
    std::cout << "Action: Play with the AmiPixel!" << std::endl; // Debug message
    // Future logic to affect happiness/fun need
}

void do_action_pet() {
    std::cout << "Action: Pet the AmiPixel!" << std::endl; // Debug message
    // Future logic to affect affection/social need
}