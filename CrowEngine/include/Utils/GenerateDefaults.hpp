#pragma once
#include "Config/PipelineSettings.hpp"
#include "Config/RenderPassSettings.hpp"
#include "ConfigPaths.hpp"
#include <cppr.hpp>

inline RenderPassConfig GenerateDefaultRenderPassConfig()
{
  RenderPassConfig renderPassConfig{"default", 1, 1};

  SerializeInternalStructsToFile(DefaultRenderpass_json,
                                 &renderPassConfig,
                                 renderPassConfig.attachments.data(),
                                 renderPassConfig.subpasses.data());

  return renderPassConfig;
}

inline PipelineSettings GenerateDefaultPipelineConfig()
{
  PipelineSettings pipelineSettings{};
  SerializeStructsToFile(DefaultPipeline_json, &pipelineSettings);
  return pipelineSettings;
}