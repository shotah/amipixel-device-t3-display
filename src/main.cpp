#include <Arduino.h>
#include <WiFi.h>
#include <TFT_eSPI.h>
#include <SPI.h>
#include <time.h>
#include "AmiPixel.h"
#include "AmiPixelUI.h"
#include "Weather.h"
#include "credentials.h"
#include <Extensions/Touch.h>

// Display settings
TFT_eSPI tft;
TFT_eSprite sprite(&tft);

// For touch input
uint16_t touchX, touchY;
bool touched = false;

// Icons
// #include "icons/icon.h"
// #include "icons/dog.h"

// System objects
AmiPixel* avatar = nullptr;
AmiPixelUI* ui = nullptr;
Weather* weather = nullptr;

// Time variables
static uint32_t lastTimeCheck = 0;
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = -28800;  // PST offset (UTC-8)
const int   daylightOffset_sec = 3600; // PDT offset when active

// Function prototypes
void connectToWiFi();
void setupTime();
void updateAndDisplayTime();
void drawBasicInterface();

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println("AmiPixel starting up...");
  
  // Initialize display
  tft.init();
  tft.setRotation(1); // Landscape orientation
  tft.fillScreen(TFT_BLACK);
  
  // Show startup message
  tft.setCursor(10, 10);
  tft.setTextSize(2);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.println("AmiPixel starting...");
  
  // Connect to WiFi
  connectToWiFi();
  
  // Set up time synchronization
  setupTime();
  
  // Draw basic interface
  drawBasicInterface();
  
  // Create AmiPixel avatar
  avatar = new AmiPixel(&tft);
  avatar->init();
  
  // Create UI
  ui = new AmiPixelUI(&tft, avatar);
  ui->init();
  
  // Create Weather object
  weather = new Weather(&tft, OPENWEATHERMAP_API_KEY, CITY_ID, CITY_NAME);
  weather->init();
  
  Serial.println("AmiPixel initialized successfully");
}

void loop() {
  // Update time display periodically
  updateAndDisplayTime();
  
  // Update weather periodically
  if (weather) {
    weather->update();
  }
  
  // Check for touch input
  touched = tft.getTouch(&touchX, &touchY);
  
  if (touched) {
    Serial.printf("Touch at x:%d y:%d\n", touchX, touchY);
    if (ui) {
      ui->handleTouch(touchX, touchY);
    }
  }
  
  // Update avatar state
  if (avatar) {
    avatar->update();
  }
  
  // Update UI
  if (ui) {
    ui->update();
  }
  
  // Small delay
  delay(50);
}

void connectToWiFi() {
  tft.println("Connecting to WiFi...");
  Serial.print("Connecting to WiFi");
  
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    Serial.print(".");
    attempts++;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    
    tft.println("WiFi connected!");
    tft.println(WiFi.localIP());
  } else {
    Serial.println("");
    Serial.println("WiFi connection failed");
    tft.println("WiFi connection failed");
  }
}

void setupTime() {
  tft.println("Setting up time...");
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    tft.println("Failed to obtain time");
    return;
  }
  
  Serial.println("Time synchronized");
  tft.println("Time synchronized");
}

void updateAndDisplayTime() {
  // Update time every second
  if (millis() - lastTimeCheck >= 1000) {
    struct tm timeinfo;
    if (getLocalTime(&timeinfo)) {
      char timeStr[9];
      strftime(timeStr, sizeof(timeStr), "%H:%M:%S", &timeinfo);
      
      // Display time
      tft.setTextSize(2);
      tft.setTextColor(TFT_WHITE, TFT_BLACK);
      tft.setCursor(tft.width() - 120, 10);
      tft.print(timeStr);
    }
    
    lastTimeCheck = millis();
  }
}

void drawBasicInterface() {
  tft.fillScreen(TFT_BLACK);
  
  // Draw header bar
  tft.fillRect(0, 0, tft.width(), 30, TFT_DARKGREY);
  
  // Add any other basic UI elements here
  // This should be minimal as the UI class will handle most UI elements
}