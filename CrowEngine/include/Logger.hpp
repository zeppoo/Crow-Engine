#pragma once

#include "crow_lib.hpp"

namespace crowe
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

  void INFO(std::string msg);

  void WARNING(std::string msg);

  void ERROR(std::string msg);

  void FATAL_ERROR(std::string msg);

  void SHUTDOWN_APP(std::string msg);

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