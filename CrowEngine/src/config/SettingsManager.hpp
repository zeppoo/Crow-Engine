#pragma once
#include <iostream>
#include <unordered_map>

namespace GeneralSettings
{
  struct EngineConfig
  {
    std::string engineName = "Crow Engine";
    std::string version = "0.0.1";
    std::string credit = "Rein Verhaag, Clement Malaka";
  };

  struct WindowConfig
  {
    int width = 800;
    int height = 600;
    bool fullscreen = false;
    bool resizable = true;
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
  struct ShaderConfig
  {
    std::string vertexShader = "path/to/vertexShader.vert";
    std::string fragmentShader = "path/to/fragmentShader.frag";
  };

  struct ShaderSettingsConfig
  {
    int timeMultiplier = 1;
    int hueMultiplier = 1;
  };
}

namespace DebugSettings
{
  struct LogConfig
  {
    std::unordered_map<int, bool> loggingLevelSettings =
        { {0, false},
          {1, false},
          {2, false} };
    std::string logFile = "path/to/log.txt";
  };
}

namespace crowe
{

  class SettingsManager
  {
  public:
    static SettingsManager& getInstance();
    static void destroyInstance();
    // General Settings
    GeneralSettings::EngineConfig engineConfig;
    GeneralSettings::WindowConfig windowConfig;
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

    static SettingsManager instance;
  };
}
