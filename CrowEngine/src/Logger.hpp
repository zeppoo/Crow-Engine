#pragma once

#include "crow_lib.hpp"

namespace crowe
{
  enum LogType
  {
    NONE,
    INFO,
    WARNING,
    ERROR,
  };

  enum LogSource
  {
    UNKOWN,
    APP,
    VULKAN,
    GUI,
    ENGINE
  };

  #ifdef NDEBUG // NDEBUG = No Debug
    const bool enableValidationLayers = false;
  #else
    const bool enableValidationLayers = true;
  #endif

  void Log(std::string msg);
  void Log(std::string msg, LogType type);
  void Log(std::string msg, LogSource source);
  void Log(std::string msg, LogType type, LogSource source);
}