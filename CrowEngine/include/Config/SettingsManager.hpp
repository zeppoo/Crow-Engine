#pragma once

#include <iostream>
#include <vector>
#include <unordered_map>

namespace GeneralSettings
{
  struct EngineConfig {

  };

  struct WindowConfig {
    int width = 800;
    int height = 600;
    bool fullscreen = false;
    bool resizable = true;
  };

  struct QueueConfig {
    int presentQueuesCount = 1;
    int graphicsQueuesCount = 1;
    int computeQueuesCount = 1;
    int transferQueuesCount = 1;
  };
}

namespace ShaderSettings
{
  struct ShaderConfig {
    std::string vertexShader = "path/to/vertexShader.vert";
    std::string fragmentShader = "path/to/fragmentShader.frag";
  };

  struct ShaderSettingsConfig {
    int timeMultiplier = 1;
    int hueMultiplier = 1;
  };
}

namespace VulkanSettings
{
  //VkInstance


  //VkDevice

}

namespace DebugSettings
{
  struct LogConfig {
    std::unordered_map<int, bool> loggingLevelSettings =
        {{0, true},
         {1, true},
         {2, true},
         {3, true}};

    std::string logFile = "path/to/log.txt";
  };
}



namespace settings
{
  GeneralSettings::EngineConfig &getEngineConfig();

  GeneralSettings::WindowConfig &getWindowConfig();

  GeneralSettings::QueueConfig &getQueueConfig();

  ShaderSettings::ShaderConfig &getShaderConfig();

  ShaderSettings::ShaderSettingsConfig &getShaderSettingsConfig();

  DebugSettings::LogConfig &getLogConfig();

  const bool getEnableValidationLayers();

  void StartRunning();

  void StopRunning();


  class SettingsManager {
  public:
    static SettingsManager &getInstance()
    { return instance; };

    // General Settings
    GeneralSettings::EngineConfig engineConfig;
    GeneralSettings::WindowConfig windowConfig;
    GeneralSettings::QueueConfig queueConfig;
    // Shader Settings
    ShaderSettings::ShaderConfig shaderConfig;
    ShaderSettings::ShaderSettingsConfig shaderSettingsConfig;
    // Debug Settings
    DebugSettings::LogConfig logConfig;
#ifdef NDEBUG // NDEBUG = No Debug
    const bool enableValidationLayers = false;
#else
    const bool enableValidationLayers = true;
#endif

  private:
    SettingsManager();

    SettingsManager(const SettingsManager &) = delete;

    SettingsManager &operator=(const SettingsManager &) = delete;

    static SettingsManager instance;
  };
}
