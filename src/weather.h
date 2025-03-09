#ifndef WEATHER_MODULE_H
#define WEATHER_MODULE_H

#include <Arduino.h>
#include <string> // Include for std::string

namespace Weather {
struct WeatherApiData // Renamed and updated struct name, using modern C++
                      // struct definition
{
  std::string city;        // Using std::string
  std::string description; // Using std::string
  double temperature;
  double temp_min;
  double temp_max;
  double pressure;
  double humidity;
};

void setupWeather(); // Setup function for the weather module
extern WeatherApiData
    weatherApi; // Declaration of the global weatherApiData instance (still
                // needed for external access to data)

} // namespace Weather

#endif // WEATHER_H