// AmiPixelUI.cpp
#include "AmiPixelUI.h"

AmiPixelUI::AmiPixelUI(TFT_eSPI* display, AmiPixel* amiPixel) :
  tft(display),
  avatar(amiPixel),
  currentScreen(UIScreen::MAIN)
{
  // Define UI element positions (adjust based on your display)
  int screenW = tft->width();
  int screenH = tft->height();
  
  // Menu buttons (positioned at bottom of screen)
  int buttonW = screenW / 4;
  int buttonH = 40;
  int buttonY = screenH - buttonH;
  
  feedButtonX = 0;
  feedButtonY = buttonY;
  feedButtonW = buttonW;
  feedButtonH = buttonH;
  
  playButtonX = buttonW;
  playButtonY = buttonY;
  playButtonW = buttonW;
  playButtonH = buttonH;
  
  sleepButtonX = buttonW * 2;
  sleepButtonY = buttonY;
  sleepButtonW = buttonW;
  sleepButtonH = buttonH;
  
  settingsButtonX = buttonW * 3;
  settingsButtonY = buttonY;
  settingsButtonW = buttonW;
  settingsButtonH = buttonH;
  
  // Back button (top-left in submenus)
  backButtonX = 0;
  backButtonY = 0;
  backButtonW = 60;
  backButtonH = 30;
}

void AmiPixelUI::init() {
  tft->fillScreen(TFT_BLACK);
  navigateTo(UIScreen::MAIN);
}

void AmiPixelUI::update() {
  // Update current screen
  switch (currentScreen) {
    case UIScreen::MAIN:
      drawMainScreen();
      break;
    case UIScreen::MENU:
      drawMenuScreen();
      break;
    case UIScreen::SETTINGS:
      drawSettingsScreen();
      break;
  }
}

void AmiPixelUI::handleTouch(int touchX, int touchY) {
  if (currentScreen == UIScreen::MAIN) {
    // Check if touch is on avatar area (center of screen)
    int avatarAreaX = tft->width() / 4;
    int avatarAreaY = tft->height() / 4;
    int avatarAreaW = tft->width() / 2;
    int avatarAreaH = tft->height() / 2;
    
    if (touchX >= avatarAreaX && touchX < (avatarAreaX + avatarAreaW) &&
        touchY >= avatarAreaY && touchY < (avatarAreaY + avatarAreaH)) {
      // Touch on avatar - pet action
      avatar->pet(touchX, touchY);
    }
    
    // Check menu buttons
    if (touchY >= feedButtonY && touchY < (feedButtonY + feedButtonH)) {
      if (touchX >= feedButtonX && touchX < (feedButtonX + feedButtonW)) {
        avatar->feed();
      } else if (touchX >= playButtonX && touchX < (playButtonX + playButtonW)) {
        avatar->play();
      } else if (touchX >= sleepButtonX && touchX < (sleepButtonX + sleepButtonW)) {
        avatar->toggleSleep();
      } else if (touchX >= settingsButtonX && touchX < (settingsButtonX + settingsButtonW)) {
        navigateTo(UIScreen::SETTINGS);
      }
    }
  } else if (currentScreen == UIScreen::SETTINGS) {
    // Check back button
    if (touchX >= backButtonX && touchX < (backButtonX + backButtonW) &&
        touchY >= backButtonY && touchY < (backButtonY + backButtonH)) {
      navigateTo(UIScreen::MAIN);
    }
    
    // Handle other settings controls
    // (brightness slider, device name, etc.)
  }
}

void AmiPixelUI::navigateTo(UIScreen screen) {
  currentScreen = screen;
  tft->fillScreen(TFT_BLACK);
  
  // Initial draw of the new screen
  switch (currentScreen) {
    case UIScreen::MAIN:
      drawMainScreen();
      break;
    case UIScreen::MENU:
      drawMenuScreen();
      break;
    case UIScreen::SETTINGS:
      drawSettingsScreen();
      break;
  }
}

void AmiPixelUI::drawMainScreen() {
  // Draw avatar area
  
  // Draw menu buttons
  tft->fillRect(feedButtonX, feedButtonY, feedButtonW, feedButtonH, TFT_DARKGREY);
  tft->fillRect(playButtonX, playButtonY, playButtonW, playButtonH, TFT_DARKGREY);
  tft->fillRect(sleepButtonX, sleepButtonY, sleepButtonW, sleepButtonH, TFT_DARKGREY);
  tft->fillRect(settingsButtonX, settingsButtonY, settingsButtonW, settingsButtonH, TFT_DARKGREY);
  
  // Draw button labels
  tft->setTextColor(TFT_WHITE, TFT_DARKGREY);
  tft->setTextSize(1);
  
  tft->setCursor(feedButtonX + 10, feedButtonY + 15);
  tft->print("Feed");
  
  tft->setCursor(playButtonX + 10, playButtonY + 15);
  tft->print("Play");
  
  tft->setCursor(sleepButtonX + 10, sleepButtonY + 15);
  tft->print(avatar->getIsSleeping() ? "Wake" : "Sleep");
  
  tft->setCursor(settingsButtonX + 5, settingsButtonY + 15);
  tft->print("Settings");
  
  // Draw needs indicators
  drawNeedsIndicators();
}

void AmiPixelUI::drawMenuScreen() {
  // Not implemented yet - could be expanded for more actions
  
  // Draw back button
  tft->fillRect(backButtonX, backButtonY, backButtonW, backButtonH, TFT_DARKGREY);
  tft->setTextColor(TFT_WHITE, TFT_DARKGREY);
  tft->setTextSize(1);
  tft->setCursor(backButtonX + 10, backButtonY + 10);
  tft->print("Back");
}

void AmiPixelUI::drawSettingsScreen() {
  // Draw back button
  tft->fillRect(backButtonX, backButtonY, backButtonW, backButtonH, TFT_DARKGREY);
  tft->setTextColor(TFT_WHITE, TFT_DARKGREY);
  tft->setTextSize(1);
  tft->setCursor(backButtonX + 10, backButtonY + 10);
  tft->print("Back");
  
  // Draw settings title
  tft->setTextSize(2);
  tft->setTextColor(TFT_WHITE, TFT_BLACK);
  tft->setCursor(tft->width()/2 - 50, 10);
  tft->print("Settings");
  
  // Device name
  tft->setTextSize(1);
  tft->setCursor(20, 50);
  tft->print("Device Name:");
  tft->setCursor(120, 50);
  tft->print(avatar->getDeviceName());
  
  // Brightness control
  tft->setCursor(20, 80);
  tft->print("Brightness:");
  // Draw brightness slider - placeholder
  tft->drawRect(120, 80, 100, 15, TFT_WHITE);
  
  // Sleep mode toggle
  tft->setCursor(20, 110);
  tft->print("Sleep Mode:");
  tft->setCursor(120, 110);
  tft->print(avatar->getIsSleeping() ? "ON" : "OFF");
}

void AmiPixelUI::drawNeedsIndicators() {
  // Draw hunger indicator
  int hungerLevel = avatar->getNeedLevel(AvatarNeed::HUNGER);
  tft->setCursor(10, 10);
  tft->setTextColor(TFT_WHITE, TFT_BLACK);
  tft->setTextSize(1);
  tft->print("Hunger: ");
  tft->drawRect(70, 10, 50, 10, TFT_WHITE);
  tft->fillRect(70, 10, map(hungerLevel, 0, 100, 0, 50), 10, TFT_GREEN);
  
  // Draw boredom indicator
  int boredomLevel = avatar->getNeedLevel(AvatarNeed::BOREDOM);
  tft->setCursor(10, 25);
  tft->print("Play:   ");
  tft->drawRect(70, 25, 50, 10, TFT_WHITE);
  tft->fillRect(70, 25, map(boredomLevel, 0, 100, 0, 50), 10, TFT_BLUE);
  
  // Draw attention indicator
  int attentionLevel = avatar->getNeedLevel(AvatarNeed::ATTENTION);
  tft->setCursor(10, 40);
  tft->print("Love:   ");
  tft->drawRect(70, 40, 50, 10, TFT_WHITE);
  tft->fillRect(70, 40, map(attentionLevel, 0, 100, 0, 50), 10, TFT_RED);
}