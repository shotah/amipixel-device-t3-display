#ifndef WEATHER_H
#define WEATHER_H

#include <Arduino.h>
#include <TFT_eSPI.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <WiFi.h>

class Weather {
private:
    TFT_eSPI* tft;
    String apiKey;
    String cityId;
    String cityName;
    
    // Weather data
    String weatherDescription;
    String weatherIcon;
    float temperature;
    float feelsLike;
    int humidity;
    float windSpeed;
    
    // Last update time
    unsigned long lastWeatherUpdate;
    const unsigned long updateInterval = 10 * 60 * 1000; // 10 minutes in milliseconds
    
    // Private methods
    bool fetchWeatherData();
    void parseWeatherData(String& payload);
    void displayWeatherInfo();
    void displayTempHumidity();
    
public:
    Weather(TFT_eSPI* display, const String& apiKeyStr, const String& cityIdStr, const String& cityNameStr);
    ~Weather();
    
    void init();
    void update();
    
    // Getters for weather data
    String getWeatherDescription() const { return weatherDescription; }
    String getWeatherIcon() const { return weatherIcon; }
    float getTemperature() const { return temperature; }
    float getFeelsLike() const { return feelsLike; }
    int getHumidity() const { return humidity; }
    float getWindSpeed() const { return windSpeed; }
    
    // Force a weather update regardless of interval
    bool forceUpdate();
};

#endif // WEATHER_H