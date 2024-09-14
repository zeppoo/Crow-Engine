#pragma once
#include "config/SettingsManager.hpp"
#include "crow_lib.hpp"

namespace crowe
{
  enum LoggingLevel
  {
    INFO,
    WARNING,
    ERROR,
  };

  class Logger
  {
  public:
    static Logger& GetInstance();
    static void StopLogging();
    void Log(std::string msg, LoggingLevel type);

  private:
    Logger();
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    void ProcessLogs();
    std::string GetTime();
    void WriteToLog(std::string log);

    static Logger instance;
    SettingsManager& settings = SettingsManager::getInstance();
    static bool loggingDone;
  };

}