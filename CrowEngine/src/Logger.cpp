#include "Logger.hpp"
#include "config/SettingsManager.hpp"
#include <ctime>
#include <chrono>
#include <thread>
#include <queue>

namespace crowe
{
  Logger Logger::instance;
  Logger& logger = Logger::GetInstance();
  bool Logger::loggingDone;
  std::queue<Log> logQueue;

  void INFO(std::string msg)
  {
    msg = "| Type: INFO | Message: " + msg;
    logQueue.push({msg, info});
  }

  void WARNING(std::string msg)
  {
    msg = "| Type: WARNING | Message: " + msg;
    logQueue.push({msg, warning});
  }

  void ERROR(std::string msg)
  {
    msg = "| Type: ERROR | Message: " + msg;
    logQueue.push({msg, error});

  }

  void FATAL_ERROR(std::string msg)
  {
    msg = "| Type: FATAL ERROR | Message: " + msg;
    logQueue.push({msg, none});
    StopRunning();
  }

  void SHUTDOWN_APP(std::string msg)
  {
    msg = "| Type: SHUTDOWN | Message: " + msg;
    logQueue.push({msg, none});
    logger.StopLogging();
  }

  Logger::Logger()
  {
    loggingDone = false;
    std::thread logThread(&Logger::ProcessLogs, this);
    logThread.detach();
  }

  void Logger::StopLogging()
  {
    loggingDone = true;
  }

  void Logger::ProcessLogs()
  {
    using namespace std::literals::chrono_literals;

    while (true)
    {
      if (loggingDone && logQueue.empty()) {break;}

      for (int i = 0; i < logQueue.size(); i++)
      {
        if (getLogConfig().loggingLevelSettings[logQueue.front().lvl]) {
          std::cout << GetTime() << logQueue.front().message << std::endl;
        }
        logQueue.pop();
      }
      if(logQueue.empty())
      {
        std::this_thread::sleep_for(5s);
      } else
      {
        std::this_thread::sleep_for(0.5s);
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


}