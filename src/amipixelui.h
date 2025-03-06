// AmiPixelUI.h
#ifndef AMIPIXEL_UI_H
#define AMIPIXEL_UI_H

#include <Arduino.h>
#include <TFT_eSPI.h>
#include "AmiPixel.h"

enum class UIScreen {
  MAIN,
  MENU,
  SETTINGS
};

class AmiPixelUI {
private:
  TFT_eSPI* tft;
  AmiPixel* avatar;
  UIScreen currentScreen;
  
  // UI elements
  int feedButtonX, feedButtonY, feedButtonW, feedButtonH;
  int playButtonX, playButtonY, playButtonW, playButtonH;
  int sleepButtonX, sleepButtonY, sleepButtonW, sleepButtonH;
  int settingsButtonX, settingsButtonY, settingsButtonW, settingsButtonH;
  int backButtonX, backButtonY, backButtonW, backButtonH;
  
  // Private methods
  void drawMainScreen();
  void drawMenuScreen();
  void drawSettingsScreen();
  void drawNeedsIndicators();
  
public:
  AmiPixelUI(TFT_eSPI* display, AmiPixel* amiPixel);
  
  void init();
  void update();
  void handleTouch(int touchX, int touchY);
  void navigateTo(UIScreen screen);
};

#endif // AMIPIXEL_UI_H