#pragma once
#include "Config/PipelineSettings.hpp"
#include "Config/RenderPassSettings.hpp"
#include "ConfigPaths.hpp"
#include <cppr.hpp>

inline RenderPassConfig GenerateDefaultRenderPassConfig()
{
  RenderPassConfig renderPassConfig{"default"};
  renderPassConfig.AddSubpass();
  renderPassConfig.AddAttachment(Color);
  renderPassConfig.subpasses[0].attachments.push_back(&renderPassConfig.attachments[0]);


  SerializeInternalStructsToFile(DefaultRenderpass_json,
                                 &renderPassConfig,
                                 renderPassConfig.subpasses.data(),
                                 renderPassConfig.attachments.data());

  return renderPassConfig;
}

inline PipelineSettings GenerateDefaultPipelineConfig()
{
  PipelineSettings pipelineSettings{};
  SerializeStructsToFile(DefaultPipeline_json, &pipelineSettings);
  return pipelineSettings;
}