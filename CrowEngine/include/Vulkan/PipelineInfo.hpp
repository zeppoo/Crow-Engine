#pragma once

#include "crow_lib.hpp"
#include "glm/glm.hpp"
#include "../Config/PipelineSettings.hpp"
#include <array>

namespace vulkan
{
  struct Vertex {
    glm::vec2 pos;
    glm::vec3 color;
  };

  struct PipelineInfo {
    Vertex vertex;
    VkViewport viewport;
    VkRect2D scissor;
    VkVertexInputBindingDescription bindingDescription;
    std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions;
    VkPipelineVertexInputStateCreateInfo vertexInputInfo;
    VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
    VkPipelineTessellationStateCreateInfo tessellationInfo;
    VkPipelineViewportStateCreateInfo viewportInfo;
    VkPipelineRasterizationStateCreateInfo rasterizationInfo;
    VkPipelineMultisampleStateCreateInfo multisampleInfo;
    VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
    VkPipelineColorBlendAttachmentState colorBlendAttachmentState;
    VkPipelineColorBlendStateCreateInfo colorBlendInfo;
    VkPipelineDynamicStateCreateInfo dynamicStatesInfo;
  };

  PipelineInfo CreatePipelineInfo(PipelineSettings &settings, VkExtent2D extent);

  VkShaderModule CreateShaderModule(VkDevice device, const std::string &filepath);

  VkPipelineShaderStageCreateInfo CreateShaderStageInfo(VkShaderStageFlagBits shaderBit, VkShaderModule shaderModule);

  VkVertexInputBindingDescription CreateBindingDescription();

  std::array<VkVertexInputAttributeDescription, 2> CreateAttributeDescriptions();

  VkPipelineVertexInputStateCreateInfo CreateVertexInputStateInfo(PipelineSettings& settings, VkVertexInputBindingDescription &bindingDescription, std::array<VkVertexInputAttributeDescription, 2> &attributeDescriptions);

  VkPipelineInputAssemblyStateCreateInfo CreateInputAssemblyStateInfo(PipelineSettings& settings);

  VkPipelineTessellationStateCreateInfo CreateTessellationStateInfo(PipelineSettings& settings);

  VkViewport CreateViewPort(VkExtent2D extent);

  VkRect2D CreateScissor(VkExtent2D extent);

  VkPipelineViewportStateCreateInfo CreateViewportStateInfo(PipelineSettings& settings, VkViewport* viewport, VkRect2D* scissor);

  VkPipelineRasterizationStateCreateInfo CreateRasterizationStateInfo(PipelineSettings& settings);

  VkPipelineMultisampleStateCreateInfo CreateMultisampleStateInfo(PipelineSettings& settings);

  VkPipelineDepthStencilStateCreateInfo CreateDepthStencilStateInfo(PipelineSettings& settings);

  VkPipelineColorBlendAttachmentState CreateColorBlendAttachmentState(PipelineSettings& settings);

  VkPipelineColorBlendStateCreateInfo CreateColorBlendStateInfo(PipelineSettings& settings, const VkPipelineColorBlendAttachmentState *colorBlendAttachment);

  VkPipelineDynamicStateCreateInfo CreateDynamicStateInfo(PipelineSettings& settings);
}
