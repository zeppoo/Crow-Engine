#include "Logger.hpp"
#include <ctime>
#include <thread>
#include <queue>

namespace crowe
{
  Logger Logger::instance;
  bool Logger::loggingDone;

  std::string LogTypes[]
      {
          "INFO",
          "WARNING",
          "ERROR"
      };

  std::queue<std::string> logQueue;

  Logger::Logger()
  {
    loggingDone = false;
    std::thread logThread(&Logger::ProcessLogs, this);
    logThread.detach();
  }

  Logger& Logger::GetInstance()
  {
    return instance;
  }

  void Logger::StopLogging()
  {
    loggingDone = true;
  }

  void Logger::ProcessLogs()
  {
    while (true)
    {
      if (loggingDone && logQueue.empty()) {break;}

      for (int i = 0; i < logQueue.size(); i++)
      {
        std::cout << GetTime() << logQueue.front() << std::endl;
      }
    }
  }

  std::string Logger::GetTime()
  {
    time_t currentTime = time(nullptr);
    tm* localTime = localtime(&currentTime);
    char timeBuffer[9]; // HH:MM:SS is 8 characters + null terminator
    strftime(timeBuffer, sizeof(timeBuffer), "%H:%M:%S", localTime);
    return std::string(timeBuffer);
  }

  void Logger::WriteToLog(std::string log)
  {
    logQueue.push(log);
  }

  void Logger::Log(std::string msg, LoggingLevel type)
  {
    std::string log = "| Type: " + LogTypes[type] + "| Message: " + msg;
    if (settings.logConfig.loggingLevelSettings[type])
    {
      WriteToLog(log);
    }
  }
}