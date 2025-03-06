#ifndef WEBSERVER_MOCK_H
#define WEBSERVER_MOCK_H

#include <string>
#include <map>
#include <functional>

typedef std::function<void()> HandlerFunction;

class MockWebRequest
{
public:
  std::map<std::string, std::string> params;

  bool hasParam(const std::string &name) const
  {
    return params.find(name) != params.end();
  }

  std::string getParam(const std::string &name) const
  {
    auto it = params.find(name);
    if (it != params.end())
    {
      return it->second;
    }
    return "";
  }
};

class MockWebServer
{
public:
  MockWebServer(int port) : _started(false) {}

  void reset()
  {
    _started = false;
    _handlers.clear();
    _currentRequest = MockWebRequest();
  }

  void begin()
  {
    _started = true;
  }

  void on(const std::string &uri, HandlerFunction handler)
  {
    _handlers[uri] = handler;
  }

  void simulateRequest(const std::string &uri, const std::string &method, const std::string &queryString)
  {
    // Parse query string into parameters
    _currentRequest.params.clear();

    size_t start = 0;
    size_t end = queryString.find('&');

    while (start < queryString.length())
    {
      std::string param;
      if (end != std::string::npos)
      {
        param = queryString.substr(start, end - start);
        start = end + 1;
        end = queryString.find('&', start);
      }
      else
      {
        param = queryString.substr(start);
        start = queryString.length();
      }

      size_t equalPos = param.find('=');
      if (equalPos != std::string::npos)
      {
        std::string name = param.substr(0, equalPos);
        std::string value = param.substr(equalPos + 1);
        _currentRequest.params[name] = value;
      }
    }

    // Call the handler if exists
    auto it = _handlers.find(uri);
    if (it != _handlers.end())
    {
      _currentUri = uri;
      it->second();
    }
  }

  bool wasStarted() const
  {
    return _started;
  }

  bool hasHandler(const std::string &uri) const
  {
    return _handlers.find(uri) != _handlers.end();
  }

  MockWebRequest &currentRequest()
  {
    return _currentRequest;
  }

  std::string uri() const
  {
    return _currentUri;
  }

private:
  bool _started;
  std::string _currentUri;
  std::map<std::string, HandlerFunction> _handlers;
  MockWebRequest _currentRequest;
};

extern MockWebServer server;

#endif // WEBSERVER_MOCK_H