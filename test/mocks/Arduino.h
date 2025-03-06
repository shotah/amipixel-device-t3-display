#ifndef ARDUINO_MOCK_H
#define ARDUINO_MOCK_H

#include <string>
#include <cstdint>

// Arduino types and functions
typedef uint8_t byte;
typedef bool boolean;
#define HIGH 1
#define LOW 0
#define LED_BUILTIN 13

unsigned long millis() {
  static unsigned long fake_time = 0;
  fake_time += 100; // Advance time by 100ms each call
  return fake_time;
}

void delay(unsigned long ms) {
  // No-op in tests
}

void pinMode(uint8_t pin, uint8_t mode) {
  // No-op in tests
}

void digitalWrite(uint8_t pin, uint8_t val) {
  // No-op in tests
}

// IPAddress class
class IPAddress {
public:
  IPAddress() {}
  IPAddress(uint8_t a, uint8_t b, uint8_t c, uint8_t d) {
    octets[0] = a;
    octets[1] = b;
    octets[2] = c;
    octets[3] = d;
  }
  
  std::string toString() const {
    char buf[16];
    sprintf(buf, "%d.%d.%d.%d", octets[0], octets[1], octets[2], octets[3]);
    return std::string(buf);
  }
  
  uint8_t octets[4] = {0, 0, 0, 0};
};

#endif // ARDUINO_MOCK_H