#pragma once

#include "crow_lib.hpp"

#include <thread>

namespace logger
{
  enum LoggerState {
    LOGGER_STATE_IDLE = 0,
    LOGGER_STATE_RUNNING,
    LOGGER_STATE_SHUTDOWN
  };

  enum LoggingLevel {
    none,
    info,
    warning,
    error
  };

  struct LogInfo {
    std::string message;
    LoggingLevel lvl;
  };

  void Info(std::string msg);

  void Warning(std::string msg);

  void Error(std::string msg);

  void FatalError(std::string msg);

  void CallShutdown(std::string msg);

  class Logger {
  public:
    static Logger &GetInstance()
    { return instance; }

    void InitializeLogger();

    void PushToLog(LogInfo log);

    static void StopLogging();

    LoggerState state = LOGGER_STATE_IDLE;

  private:
    Logger(){}

    Logger(const Logger &) = delete;

    Logger &operator=(const Logger &) = delete;

    void ProcessLogs();

    std::string GetTime();

    std::thread loggingThread;
    static Logger instance;
    static bool loggingDone;
  };

}