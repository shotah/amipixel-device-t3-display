#include <string>
#ifndef CONSTANTS_H
#define CONSTANTS_H

namespace Constants { // Optional namespace to further organize
struct Display {
  static const int SCREEN_WIDTH = 64;
  static const int SCREEN_HEIGHT = 32;
  static const int SCREEN_BUFFER_SIZE = SCREEN_WIDTH * SCREEN_HEIGHT / 2;
};

struct I2C {
  static const int SDA_PIN = 21;
  static const int SCL_PIN = 20;
  static const int OLED_ADDRESS = 0x3C;
  static const int OLED_RESET_PIN = -1;
};

struct SPI {
  static const int MOSI_PIN = 11;
  static const int MISO_PIN = 12;
  static const int SCK_PIN = 13;
  static const int CS_PIN = 10;
  static const int TFT_DC_PIN = 9;
  static const int TFT_RESET_PIN = 8;
};

struct LED {
  static const int PIN = 25;
  static const int NUM = 1;
  static constexpr const char *TYPE =
      "NEO_GRBW"; // Or use an enum if LED_TYPE is meant to be a specific set of
                  // choices
  static const int DATA_PIN = PIN; // DATA_PIN is same as PIN
  static const int BRIGHTNESS = 50;
};

struct ButtonInput {
  static const int BUTTON_PIN = 0;
  static const int ADC_PIN = 26;
  static const int PWM_PIN = 16;
};

struct Timing {
  static const int FRAMES_PER_SECOND = 60;
  static const int PATTERN_DELAY_MS = 500;
};

struct ColorPalette {
  static const int NUM_COLORS = 256;
  static const int NUM_PATTERNS = 5;
};

struct SpeedBrightness {
  static const int MAX_SPEED = 100;
  static const int MIN_SPEED = 1;
  static const int SPEED_STEP = 5;
  static const int MAX_BRIGHTNESS = 255;
  static const int MIN_BRIGHTNESS = 10;
  static const int BRIGHTNESS_STEP = 10;
  static const int MAX_CONTRAST = 255;
  static const int MIN_CONTRAST = 10;
  static const int CONTRAST_STEP = 10;
  static const int MAX_SATURATION = 255;
  static const int MIN_SATURATION = 10;
  static const int SATURATION_STEP = 10;
};

struct WiFiConfig {
  static constexpr const char *SSID =
      "Your WiFi SSID"; // Use const char* for string literals
  static constexpr const char *PASSWORD =
      "Your WiFi PASSWORD"; // Use const char* for string literals
  static const int WIFI_MAX_CONNECT_RETRIES = 15;
};

struct APIKeys {
  static constexpr const char *COINMARKETCAP =
      ""; // !Your Coinmarketcap API KEY , See https://coinmarketcap.com/api/
  static constexpr const char *OPENWEATHERMAP =
      ""; // !Your OpenWeatherMap API KEY, See https://openweathermap.org/api
};

struct WeatherLocation {
  static constexpr double LATITUDE =
      47.59525100879402; // Latitude and Longitude as strings for consistency
                         // with the original code
  static constexpr double LONGITUDE = -122.30992705368763;
  static constexpr const bool USE_CELSIUS =
      false; // Or static const bool USE_FAHRENHEIT = true; if you prefer
             // Fahrenheit
  static constexpr const bool USE_FAHRENHEIT = true;
};

struct TimeConfig {
  static constexpr const char *NTP_SERVER_1 =
      "pool.ntp.org"; // Server addresses are strings
  static constexpr const char *NTP_SERVER_2 = "time.nist.gov";
  static constexpr const int GMT_OFFSET_SEC = 0;
  static constexpr const int DAY_LIGHT_OFFSET_SEC = 0;
  static constexpr const char *TIMEZONE_API_URL =
      "https://ipapi.co/timezone/"; // API URLs are strings
  static constexpr const char *COINMARKETCAP_API_HOST =
      "pro-api.coinmarketcap.com"; // Hostnames are strings
  static constexpr const char *DEFAULT_TIMEZONE_STR =
      "CST-8"; // Timezone as string
};

struct MessageIDs {
  static constexpr int COIN_MSG_ID = 0x1000;
  static constexpr int WIFI_MSG_ID = 0x1001;
  static constexpr int TEMPERATURE_MSG_ID = 0x1002;
  static constexpr int WEATHER_MSG_ID = 0x1003;

  // --- WiFi Specific Messages ---
  enum WiFiMessagesEnum : uint32_t { // Use enum class for better scoping, and
                                     // uint32_t to match lv_msg_id
    WIFI_CONNECTED_MSG = 0x1100,
    WIFI_DISCONNECTED_MSG = 0x1101,
    WIFI_GOT_IP_MSG = 0x1102,
    WIFI_LOST_IP_MSG = 0x1103
  };
};
} // namespace Constants

#endif // CONSTANTS_H