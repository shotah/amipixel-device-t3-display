// AmiPixel.h
#ifndef AMIPIXEL_H
#define AMIPIXEL_H

#include <Arduino.h>
#include <TFT_eSPI.h>
#include <vector>
#include <map>

enum class AvatarMood {
  SAD,
  NEUTRAL,
  HAPPY
};

enum class AvatarAction {
  IDLE,
  LOOK_AROUND,
  FIDGET,
  FEED,
  PLAY,
  PET,
  SLEEP,
  WAKE_UP
};

enum class AvatarNeed {
  HUNGER,
  BOREDOM,
  ATTENTION
};

class AmiPixel {
private:
  TFT_eSPI* tft;
  
  // Avatar state
  AvatarMood currentMood;
  AvatarAction currentAction;
  bool isSleeping;
  
  // Needs system
  std::map<AvatarNeed, int> needs; // 0-100 scale
  unsigned long lastInteractionTime;
  unsigned long lastAutonomousActionTime;
  
  // Animation frames
  std::map<AvatarAction, std::vector<unsigned short*>> animationFrames;
  std::map<AvatarMood, std::vector<unsigned short*>> moodFrames;
  int currentFrame;
  
  // Settings
  String deviceName;
  int brightness;
  
  // Private methods
  void updateMood();
  void depleteNeeds();
  void chooseAutonomousAction();
  void renderCurrentFrame();
  
public:
  AmiPixel(TFT_eSPI* display);
  ~AmiPixel();
  
  // Initialization
  void init();
  void loadAnimations();
  
  // Main update function (call in loop)
  void update();
  
  // User interactions
  void feed();
  void play();
  void pet(int touchX, int touchY);
  void toggleSleep();
  
  // Status functions
  AvatarMood getMood() const { return currentMood; }
  bool getIsSleeping() const { return isSleeping; }
  int getNeedLevel(AvatarNeed need) const;
  
  // Settings
  void setDeviceName(const String& name) { deviceName = name; }
  String getDeviceName() const { return deviceName; }
  void setBrightness(int level);
};

#endif // AMIPIXEL_H