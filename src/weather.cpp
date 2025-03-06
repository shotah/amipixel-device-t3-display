#include "Weather.h"

Weather::Weather(TFT_eSPI* display, const String& apiKeyStr, const String& cityIdStr, const String& cityNameStr) :
    tft(display),
    apiKey(apiKeyStr),
    cityId(cityIdStr),
    cityName(cityNameStr),
    weatherDescription(""),
    weatherIcon(""),
    temperature(0),
    feelsLike(0),
    humidity(0),
    windSpeed(0),
    lastWeatherUpdate(0)
{
    // Constructor
}

Weather::~Weather() {
    // Destructor
}

void Weather::init() {
    // Initialize with a forced update
    forceUpdate();
}

void Weather::update() {
    unsigned long currentTime = millis();
    
    // Check if it's time to update weather data
    if (currentTime - lastWeatherUpdate >= updateInterval) {
        if (fetchWeatherData()) {
            lastWeatherUpdate = currentTime;
        }
    }
}

bool Weather::fetchWeatherData() {
    // Check WiFi connection
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("No WiFi connection for weather update");
        return false;
    }
    
    // Initialize HTTP client
    HTTPClient http;
    String url = "http://api.openweathermap.org/data/2.5/weather?id=" + cityId + "&units=metric&APPID=" + apiKey;
    
    http.begin(url);
    int httpCode = http.GET();
    
    if (httpCode > 0) {
        if (httpCode == HTTP_CODE_OK) {
            String payload = http.getString();
            Serial.println("Weather data received");
            
            // Parse and store the weather data
            parseWeatherData(payload);
            
            // Display the updated weather
            displayWeatherInfo();
            
            http.end();
            return true;
        } else {
            Serial.printf("HTTP error: %d\n", httpCode);
        }
    } else {
        Serial.printf("HTTP request failed: %s\n", http.errorToString(httpCode).c_str());
    }
    
    http.end();
    return false;
}

void Weather::parseWeatherData(String& payload) {
    DynamicJsonDocument doc(1024);
    DeserializationError error = deserializeJson(doc, payload);
    
    if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.c_str());
        return;
    }
    
    // Extract weather data
    weatherDescription = doc["weather"][0]["description"].as<String>();
    weatherIcon = doc["weather"][0]["icon"].as<String>();
    temperature = doc["main"]["temp"];
    feelsLike = doc["main"]["feels_like"];
    humidity = doc["main"]["humidity"];
    windSpeed = doc["wind"]["speed"];
    
    Serial.println("Weather parsed successfully");
    Serial.print("Temp: ");
    Serial.print(temperature);
    Serial.print("°C, Description: ");
    Serial.println(weatherDescription);
}

void Weather::displayWeatherInfo() {
    // This implementation would draw weather information on the display
    // This is a placeholder - you would need to adapt this to your UI design
    
    // Clear the weather area
    // tft->fillRect(weatherX, weatherY, weatherWidth, weatherHeight, TFT_BLACK);
    
    // Display city name
    tft->setCursor(10, 10);
    tft->setTextSize(2);
    tft->setTextColor(TFT_WHITE, TFT_BLACK);
    tft->print(cityName);
    
    // Display temperature
    tft->setCursor(10, 40);
    tft->setTextSize(3);
    tft->print(String(temperature, 1));
    tft->print(" C");
    
    // Display weather description
    tft->setCursor(10, 80);
    tft->setTextSize(1);
    tft->print(weatherDescription);
    
    // You could also add code here to display weather icons
    // based on the weatherIcon value
    
    displayTempHumidity();
}

void Weather::displayTempHumidity() {
    // Display additional weather details
    tft->setCursor(10, 100);
    tft->setTextSize(1);
    tft->print("Feels like: ");
    tft->print(String(feelsLike, 1));
    tft->print(" C");
    
    tft->setCursor(10, 115);
    tft->print("Humidity: ");
    tft->print(humidity);
    tft->print("%");
    
    tft->setCursor(10, 130);
    tft->print("Wind: ");
    tft->print(String(windSpeed, 1));
    tft->print(" m/s");
}

bool Weather::forceUpdate() {
    return fetchWeatherData();
}