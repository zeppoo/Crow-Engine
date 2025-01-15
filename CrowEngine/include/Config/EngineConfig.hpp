#pragma once
#include "crow_lib.hpp"
#include "cppr.hpp"

namespace configs
{
  struct EngineConfig
  {
    REFLECT()
    std::string nameID;

    // App
    std::string appName = "Application";
    std::string engineName = "Engine";
    std::string version = "0.0.1";
    std::string credit = "Rein Verhaag, Clement Malaka";
    bool isRunning = false;

    // Window
    std::string windowTitle = appName;
    uint32_t windowWidth = 1000;
    uint32_t windowHeight = 800;
    bool fullscreen = false;
    bool vsync = false;

    // Assets
    std::string assetDirectory = "path/to/file";

    // Utils
    bool enableDebugLogging = true;
    std::string logFilePath = "path/to/file";
  };

  REFLECT_STRUCT_BEGIN(EngineConfig)
    REFLECT_STRUCT_MEMBER(nameID)
    REFLECT_STRUCT_MEMBER(appName)
    REFLECT_STRUCT_MEMBER(engineName)
    REFLECT_STRUCT_MEMBER(version)
    REFLECT_STRUCT_MEMBER(credit)
    REFLECT_STRUCT_MEMBER(isRunning)
    REFLECT_STRUCT_MEMBER(windowTitle)
    REFLECT_STRUCT_MEMBER(windowWidth)
    REFLECT_STRUCT_MEMBER(windowHeight)
    REFLECT_STRUCT_MEMBER(fullscreen)
    REFLECT_STRUCT_MEMBER(vsync)
    REFLECT_STRUCT_MEMBER(assetDirectory)
    REFLECT_STRUCT_MEMBER(enableDebugLogging)
    REFLECT_STRUCT_MEMBER(logFilePath)
  REFLECT_STRUCT_END()
}