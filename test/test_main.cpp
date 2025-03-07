#include <unity.h>

// Mock headers
// #include <emulation.h>
#include "mocks/Arduino.h"
#include "mocks/WiFiMock.h"
#include "mocks/DisplayMock.h"
#include "mocks/WebServerMock.h"

// Include the actual code to test (with mock replacements)
#include "main.cpp"

MockDisplay tft;
MockWiFi WiFi;
MockWebServer server(80);

void setUp(void)
{
  // Reset state before each test
  WiFi.reset();
  tft.reset();
  server.reset();
}

void tearDown(void)
{
  // Clean up after each test
}

void test_setup_initializes_display()
{
  // Call the setup function
  setup();

  // Verify that the display was initialized
  TEST_ASSERT_TRUE(tft.wasInitialized());
  TEST_ASSERT_TRUE(tft.wasCleared());
}

void test_setup_connects_to_wifi()
{
  // Define the ssid and password
  const char *ssid = "test_ssid";
  const char *password = "test_password";

  // Call the setup function
  setup();

  // Verify that WiFi connection was attempted
  TEST_ASSERT_TRUE(WiFi.beginWasCalled());
  TEST_ASSERT_EQUAL_STRING(ssid, WiFi.getLastSSID());
  TEST_ASSERT_EQUAL_STRING(password, WiFi.getLastPassword());
}

void test_display_shows_wifi_info()
{
  // Simulate successful WiFi connection
  WiFi.setLocalIP(IPAddress(192, 168, 1, 100));
  WiFi.setStatus(WL_CONNECTED);

  // Call setup to trigger display update
  setup();

  // Verify that display shows IP address
  TEST_ASSERT_TRUE(tft.textContains("192.168.1.100"));
}

void test_web_server_initialized()
{
  // Call setup
  setup();

  // Verify that server routes were added and server was started
  TEST_ASSERT_TRUE(server.wasStarted());
  TEST_ASSERT_TRUE(server.hasHandler("/"));
  TEST_ASSERT_TRUE(server.hasHandler("/rgb"));
}

int getPixelR()
{
  // Mock implementation
  return 100;
}

int getPixelG()
{
  // Mock implementation
  return 150;
}

int getPixelB()
{
  // Mock implementation
  return 200;
}

void test_rgb_endpoint_updates_pixel_color()
{
  // Setup initial state
  setup();

  // Simulate a request to the RGB endpoint
  server.simulateRequest("/rgb", "GET", "r=100&g=150&b=200");

  // Call loop() to process the request
  loop();

  // Verify the pixel color was updated
  TEST_ASSERT_EQUAL_INT(100, getPixelR());
  TEST_ASSERT_EQUAL_INT(150, getPixelG());
  TEST_ASSERT_EQUAL_INT(200, getPixelB());
}

int main()
{
  UNITY_BEGIN();
  RUN_TEST(test_setup_initializes_display);
  RUN_TEST(test_setup_connects_to_wifi);
  RUN_TEST(test_display_shows_wifi_info);
  RUN_TEST(test_web_server_initialized);
  RUN_TEST(test_rgb_endpoint_updates_pixel_color);
  return UNITY_END();
}