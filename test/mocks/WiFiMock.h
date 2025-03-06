#ifndef WIFI_MOCK_H
#define WIFI_MOCK_H

#include <string>
#include "Arduino.h"

#define WL_CONNECTED 3
#define WL_CONNECT_FAILED 4
#define WL_DISCONNECTED 6

class MockWiFi {
public:
  MockWiFi() : _status(WL_DISCONNECTED), _beginCalled(false) {}
  
  void reset() {
    _beginCalled = false;
    _status = WL_DISCONNECTED;
    _ssid.clear();
    _password.clear();
  }
  
  int begin(const char* ssid, const char* password) {
    _beginCalled = true;
    _ssid = ssid;
    _password = password;
    
    // Simulate connection success
    _status = WL_CONNECTED;
    return _status;
  }
  
  void setStatus(int status) {
    _status = status;
  }
  
  int status() {
    return _status;
  }
  
  IPAddress localIP() {
    return _localIP;
  }
  
  void setLocalIP(const IPAddress& ip) {
    _localIP = ip;
  }
  
  bool beginWasCalled() const {
    return _beginCalled;
  }
  
  const char* getLastSSID() const {
    return _ssid.c_str();
  }
  
  const char* getLastPassword() const {
    return _password.c_str();
  }
  
private:
  int _status;
  bool _beginCalled;
  std::string _ssid;
  std::string _password;
  IPAddress _localIP;
};

extern MockWiFi WiFi;

#endif // WIFI_MOCK_H