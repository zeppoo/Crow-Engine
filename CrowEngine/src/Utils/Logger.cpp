#include "Utils/Logger.hpp"
#include <chrono>
#include <ctime>
#include <mutex>
#include <queue>
#include <condition_variable>
#include <atomic>
#include "Config/SettingsManager.hpp"

namespace logger
{
  std::atomic<bool> stop_flag{false};
  Logger Logger::instance;
  Logger& logger = Logger::GetInstance();
  bool Logger::loggingDone;
  std::queue<LogInfo> logQueue;
  std::condition_variable cv;
  std::mutex mtx;

  void Info(std::string msg)
  {
    msg = "| Type: INFO | Message: " + msg;
    logger.PushToLog({msg, info});
  }

  void Warning(std::string msg)
  {
    msg = "| Type: WARNING | Message: " + msg;
    logger.PushToLog({msg, warning});
  }

  void Error(std::string msg)
  {
    msg = "| Type: ERROR | Message: " + msg;
    logger.PushToLog({msg, error});
  }

  void FatalError(std::string msg)
  {
    msg = "| Type: FATAL ERROR | Message: " + msg;
    logger.PushToLog({msg, none});
    CallShutdown("Fatal Error!");
  }

  void CallShutdown(std::string msg)
  {
    msg = "| Type: SHUTDOWN | Message: " + msg;
    logger.PushToLog({msg, none});
    logger.StopLogging();
  }

  void Logger::InitializeLogger()
  {
    loggingDone = false;
    loggingThread = std::thread(&Logger::ProcessLogs, this);
    loggingThread.detach();
  }

  void Logger::PushToLog(LogInfo log)
  {
    logQueue.push(log);
    cv.notify_one();
  }

  void Logger::StopLogging()
  {
    loggingDone = true;
  }

  void Logger::ProcessLogs()
  {
    using namespace std::literals::chrono_literals;
    std::unique_lock<std::mutex> lock(mtx);

    while (true)
    {
      // Wait until there are messages or logging is done
      cv.wait(lock, [this]() {
          return !logQueue.empty() || loggingDone;
      });

      if (loggingDone && logQueue.empty()) {
        break;
      }

      // Process all current messages
      while (!logQueue.empty())
      {
        if (settings::getLogConfig().loggingLevelSettings[logQueue.front().lvl]) {
          std::cout << GetTime() << logQueue.front().message << std::endl;
        }
        logQueue.pop();
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

