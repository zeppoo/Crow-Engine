#pragma once

#include <iostream>
#include <unordered_map>

namespace GeneralSettings
{
  struct EngineConfig {
    std::string engineName = "Crow Engine";
    std::string version = "0.0.1";
    std::string credit = "Rein Verhaag, Clement Malaka";
    bool isRunning = false;
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

namespace RenderingSettings
{
  struct MultiSampling {
    bool enabled = true;
    int samples = 4;
  };

  struct Shadows {
    bool enabled = false;
    std::string quality = "None";
  };

  struct Textures {
    bool mipmaps = true;
    int anisotropicFiltering = 0;
  };

  struct Lighting {
    bool globalIllumination = false;
    float ambientLight[3] = {0, 0, 0};
  };

  struct Bloom {
    bool enabled = false;
    float threshold = 0;
  };

  struct PostProcessing {
    Bloom bloom;
    bool motionBlur = false;
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

namespace crowe
{
  GeneralSettings::EngineConfig &getEngineConfig();

  GeneralSettings::WindowConfig &getWindowConfig();

  GeneralSettings::QueueConfig &getQueueConfig();

  RenderingSettings::MultiSampling &getMultiSamplingConfig();

  RenderingSettings::Shadows &getShadowsConfig();

  RenderingSettings::Textures &getTexturesConfig();

  RenderingSettings::Lighting &getLightingConfig();

  RenderingSettings::PostProcessing &getPostProcessingConfig();

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
    // Rendering Settings
    RenderingSettings::MultiSampling multiSamplingConfig;
    RenderingSettings::Shadows shadowsConfig;
    RenderingSettings::Textures texturesConfig;
    RenderingSettings::Lighting lightingConfig;
    RenderingSettings::PostProcessing postProcessingConfig;
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
