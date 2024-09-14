#include "Logger.hpp"
#include <time.h>

namespace crowe
{
  std::string LogTypes[]
      {
          "NONE",
          "INFO",
          "WARNING",
          "ERROR",
      };

  std::string LogSources[]
      {
          "UNKOWN",
          "APP",
          "VULKAN",
          "GUI",
          "ENGINE"
      };

  std::string GetTime()
  {
    time_t currentTime = time(nullptr);
    tm* localTime = localtime(&currentTime);
    char timeBuffer[9]; // HH:MM:SS is 8 characters + null terminator
    strftime(timeBuffer, sizeof(timeBuffer), "%H:%M:%S", localTime);
    return std::string(timeBuffer);
  }

  void WriteToLog(std::string log)
  {
    std::cout << GetTime() << log << std::endl;
  }

  void Log(std::string msg)
  {
    std::string log = "| Type: " + LogTypes[NONE] + "| Source: " + LogSources[UNKOWN] + "| Message: " + msg;
    WriteToLog(log);
  }

  void Log(std::string msg, LogType type)
  {
    std::string log = "| Type: " + LogTypes[type] + "| Source: " + LogSources[UNKOWN] + "| Message: " + msg;
    WriteToLog(log);
  }

  void Log(std::string msg, LogSource source)
  {
    std::string log = "| Type: " + LogTypes[NONE] + "| Source: " + LogSources[source] + "| Message: " + msg;
    WriteToLog(log);
  }

  void Log(std::string msg, LogType type, LogSource source)
  {
    std::string log = "| Type: " + LogTypes[type] + "| Source: " + LogSources[source] + "| Message: " + msg;
    WriteToLog(log);
  }
}