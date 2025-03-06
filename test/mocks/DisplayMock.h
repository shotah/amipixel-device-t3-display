#ifndef DISPLAY_MOCK_H
#define DISPLAY_MOCK_H

#include <string>
#include <vector>
#include "Arduino.h"

// Font definitions to match TFT_eSPI
#define GFXFF 1
#define FF18 4

class MockDisplay {
public:
  MockDisplay() : _initialized(false), _cleared(false) {}
  
  void reset() {
    _initialized = false;
    _cleared = false;
    _text.clear();
  }
  
  void init() {
    _initialized = true;
  }
  
  void fillScreen(uint16_t color) {
    _cleared = true;
  }
  
  void setTextColor(uint16_t color) {
    // No-op in tests
  }
  
  void setTextSize(uint8_t size) {
    // No-op in tests
  }
  
  void setFreeFont(const void* font) {
    // No-op in tests
  }
  
  void setCursor(int16_t x, int16_t y) {
    _cursorX = x;
    _cursorY = y;
  }
  
  void println(const char* text) {
    _text.push_back(std::string(text));
  }
  
  void println(const std::string& text) {
    _text.push_back(text);
  }
  
  void println(int value) {
    char buf[16];
    sprintf(buf, "%d", value);
    _text.push_back(std::string(buf));
  }
  
  void drawCircle(int16_t x, int16_t y, int16_t r, uint16_t color) {
    // No-op in tests
  }
  
  void fillCircle(int16_t x, int16_t y, int16_t r, uint16_t color) {
    // No-op in tests
  }
  
  bool wasInitialized() const {
    return _initialized;
  }
  
  bool wasCleared() const {
    return _cleared;
  }
  
  bool textContains(const std::string& substr) const {
    for (const auto& line : _text) {
      if (line.find(substr) != std::string::npos) {
        return true;
      }
    }
    return false;
  }
  
private:
  bool _initialized;
  bool _cleared;
  int16_t _cursorX;
  int16_t _cursorY;
  std::vector<std::string> _text;
};

extern MockDisplay tft;

#endif // DISPLAY_MOCK_H