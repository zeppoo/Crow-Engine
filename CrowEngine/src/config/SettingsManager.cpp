#include "../../include/Config/SettingsManager.hpp"

namespace crowe
{
  SettingsManager SettingsManager::instance;
  SettingsManager& settings = SettingsManager::getInstance();

  GeneralSettings::EngineConfig& getEngineConfig(){return settings.engineConfig;}
  GeneralSettings::WindowConfig& getWindowConfig(){return settings.windowConfig;}
  GeneralSettings::QueueConfig& getQueueConfig(){return settings.queueConfig;}
  RenderingSettings::MultiSampling& getMultiSamplingConfig(){return settings.multiSamplingConfig;}
  RenderingSettings::Shadows& getShadowsConfig(){return settings.shadowsConfig;}
  RenderingSettings::Textures& getTexturesConfig(){return settings.texturesConfig;}
  RenderingSettings::Lighting& getLightingConfig(){return settings.lightingConfig;}
  RenderingSettings::PostProcessing& getPostProcessingConfig(){return settings.postProcessingConfig;}
  ShaderSettings::ShaderConfig& getShaderConfig(){return settings.shaderConfig;}
  ShaderSettings::ShaderSettingsConfig& getShaderSettingsConfig(){return settings.shaderSettingsConfig;}
  DebugSettings::LogConfig& getLogConfig(){return settings.logConfig;}
  const bool getEnableValidationLayers() {return settings.enableValidationLayers;}
  void StartRunning() {settings.engineConfig.isRunning = true;}
  void StopRunning() {settings.engineConfig.isRunning = false;}

  SettingsManager::SettingsManager() {}
}
