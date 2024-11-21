#pragma once

#include "crow_lib.hpp"

namespace log
{
  enum LoggingLevel {
    none,
    info,
    warning,
    error
  };

  struct Log {
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

    void PushToLog(Log log);

    static void StopLogging();

  private:
    Logger();

    Logger(const Logger &) = delete;

    Logger &operator=(const Logger &) = delete;

    void ProcessLogs();

    std::string GetTime();

    static Logger instance;
    static bool loggingDone;
  };

}