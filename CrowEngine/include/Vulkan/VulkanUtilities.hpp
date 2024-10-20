#pragma once

#include "crow_lib.hpp"
#include "../Config/PipelineSettings.hpp"

namespace crowe
{
  struct Info {
    int key;
    std::string name;
    std::vector<double> infos;
  };

  struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
  };

  struct PipelineInfo {
    VkViewport viewport;
    VkRect2D scissor;
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

  PipelineInfo CreatePipelineInfo(PipelineSettings settings, VkExtent2D extent);

  SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface);

  VkShaderModule CreateShaderModule(VkDevice device, const std::string &filepath);

  VkPipelineShaderStageCreateInfo CreateShaderStageInfo(VkShaderStageFlagBits shaderBit, VkShaderModule shaderModule);

  VkPipelineVertexInputStateCreateInfo CreateVertexInputStateInfo(PipelineSettings& settings);

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
