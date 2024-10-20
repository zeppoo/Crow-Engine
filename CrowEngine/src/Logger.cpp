#include "Logger.hpp"
#include "Config/SettingsManager.hpp"
#include <ctime>
#include <chrono>
#include <thread>
#include <queue>
#include <mutex>

namespace crowe
{
  Logger Logger::instance;
  Logger& logger = Logger::GetInstance();
  bool Logger::loggingDone;
  std::queue<Log> logQueue;
  std::mutex mtx;

  void INFO(std::string msg)
  {
    msg = "| Type: INFO | Message: " + msg;
    logger.PushToLog({msg, info});
  }

  void WARNING(std::string msg)
  {
    msg = "| Type: WARNING | Message: " + msg;
    logger.PushToLog({msg, warning});
  }

  void ERROR(std::string msg)
  {
    msg = "| Type: ERROR | Message: " + msg;
    logger.PushToLog({msg, error});
  }

  void FATAL_ERROR(std::string msg)
  {
    msg = "| Type: FATAL ERROR | Message: " + msg;
    logger.PushToLog({msg, none});
    StopRunning();
  }

  void SHUTDOWN_APP(std::string msg)
  {
    msg = "| Type: SHUTDOWN | Message: " + msg;
    logger.PushToLog({msg, none});
    logger.StopLogging();
  }

  Logger::Logger()
  {
    loggingDone = false;
    std::thread logThread(&Logger::ProcessLogs, this);
    logThread.detach();
  }

  void Logger::PushToLog(Log log)
  {
    logQueue.push(log);
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

      mtx.lock();
      for (int i = 0; i < logQueue.size(); i++)
      {
        if (getLogConfig().loggingLevelSettings[logQueue.front().lvl]) {
          std::cout << GetTime() << logQueue.front().message << std::endl;
        }
        logQueue.pop();
      }
      mtx.unlock();

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