// AmiPixel.cpp
#include "AmiPixel.h"

// Constants
const unsigned long AUTONOMOUS_ACTION_INTERVAL = 5000; // ms
const unsigned long NEED_DEPLETION_INTERVAL = 30000; // ms
const int NEED_DEPLETION_RATE = 5; // points
const int INTERACTION_BOOST = 25; // points

AmiPixel::AmiPixel(TFT_eSPI* display) : 
  tft(display),
  currentMood(AvatarMood::NEUTRAL),
  currentAction(AvatarAction::IDLE),
  isSleeping(false),
  currentFrame(0),
  deviceName("AmiPixel"),
  brightness(100),
  lastInteractionTime(0),
  lastAutonomousActionTime(0)
{
  // Initialize needs to 50%
  needs[AvatarNeed::HUNGER] = 50;
  needs[AvatarNeed::BOREDOM] = 50;
  needs[AvatarNeed::ATTENTION] = 50;
}

AmiPixel::~AmiPixel() {
  // Free animation memory if needed
}

void AmiPixel::init() {
  lastInteractionTime = millis();
  lastAutonomousActionTime = millis();
  loadAnimations();
}

void AmiPixel::loadAnimations() {
  // This would load animation frames from SPIFFS or PROGMEM
  // For now, we'll leave as placeholder for future implementation
  // You'll need to replace this with actual sprite/animation loading
  
  // Example (pseudocode):
  // animationFrames[AvatarAction::IDLE].push_back(idle_frame1);
  // animationFrames[AvatarAction::IDLE].push_back(idle_frame2);
  // etc.
}

void AmiPixel::update() {
  unsigned long currentTime = millis();
  
  // Skip updates if sleeping (except wake check)
  if (isSleeping) {
    // Render sleep animation
    return;
  }
  
  // Update needs every NEED_DEPLETION_INTERVAL
  if (currentTime - lastInteractionTime >= NEED_DEPLETION_INTERVAL) {
    depleteNeeds();
    updateMood();
    lastInteractionTime = currentTime;
  }
  
  // Choose a new autonomous action periodically
  if (currentTime - lastAutonomousActionTime >= AUTONOMOUS_ACTION_INTERVAL) {
    chooseAutonomousAction();
    lastAutonomousActionTime = currentTime;
  }
  
  // Render current animation frame
  renderCurrentFrame();
  
  // Advance animation frame
  currentFrame = (currentFrame + 1) % animationFrames[currentAction].size();
}

void AmiPixel::depleteNeeds() {
  for (auto& need : needs) {
    need.second = max(0, need.second - NEED_DEPLETION_RATE);
  }
}

void AmiPixel::updateMood() {
  // Calculate average need level
  int totalNeeds = 0;
  for (const auto& need : needs) {
    totalNeeds += need.second;
  }
  
  int avgNeed = totalNeeds / needs.size();
  
  // Update mood based on average need level
  if (avgNeed < 30) {
    currentMood = AvatarMood::SAD;
  } else if (avgNeed < 70) {
    currentMood = AvatarMood::NEUTRAL;
  } else {
    currentMood = AvatarMood::HAPPY;
  }
}

void AmiPixel::chooseAutonomousAction() {
  // Choose a random autonomous action based on current mood
  int actionChoice = random(3); // 0-2
  
  switch (actionChoice) {
    case 0:
      currentAction = AvatarAction::IDLE;
      break;
    case 1:
      currentAction = AvatarAction::LOOK_AROUND;
      break;
    case 2:
      currentAction = AvatarAction::FIDGET;
      break;
  }
  
  // Reset animation frame
  currentFrame = 0;
}

void AmiPixel::renderCurrentFrame() {
  // This would render the current animation frame to the display
  // For now, we'll leave as placeholder
  
  // Example:
  // if (!animationFrames[currentAction].empty()) {
  //   tft->pushImage(x, y, width, height, animationFrames[currentAction][currentFrame]);
  // }
}

void AmiPixel::feed() {
  needs[AvatarNeed::HUNGER] = min(100, needs[AvatarNeed::HUNGER] + INTERACTION_BOOST);
  currentAction = AvatarAction::FEED;
  currentFrame = 0;
  lastInteractionTime = millis();
  updateMood();
}

void AmiPixel::play() {
  needs[AvatarNeed::BOREDOM] = min(100, needs[AvatarNeed::BOREDOM] + INTERACTION_BOOST);
  currentAction = AvatarAction::PLAY;
  currentFrame = 0;
  lastInteractionTime = millis();
  updateMood();
}

void AmiPixel::pet(int touchX, int touchY) {
  needs[AvatarNeed::ATTENTION] = min(100, needs[AvatarNeed::ATTENTION] + INTERACTION_BOOST);
  currentAction = AvatarAction::PET;
  currentFrame = 0;
  lastInteractionTime = millis();
  updateMood();
  
  // Could add special reaction based on where the touch happened
  // e.g., if (touchY < head_top && touchY > head_bottom) {...}
}

void AmiPixel::toggleSleep() {
  if (isSleeping) {
    isSleeping = false;
    currentAction = AvatarAction::WAKE_UP;
  } else {
    isSleeping = true;
    currentAction = AvatarAction::SLEEP;
  }
  currentFrame = 0;
}

void AmiPixel::setBrightness(int level) {
  brightness = constrain(level, 0, 100);
  // Implement actual brightness control for your specific display
}

int AmiPixel::getNeedLevel(AvatarNeed need) const {
  if (needs.find(need) != needs.end()) {
    return needs.at(need);
  }
  return 0;
}