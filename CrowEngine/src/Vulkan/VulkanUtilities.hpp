#pragma once

#include "../crow_lib.hpp"


namespace crowe
{
  struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
  };

  struct PipelineCreateInfo {
    VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
    VkPipelineTessellationStateCreateInfo tessellationInfo;
    VkPipelineVertexInputStateCreateInfo vertexInputInfo;
    VkPipelineViewportStateCreateInfo viewportInfo;
    VkPipelineShaderStageCreateInfo shaderStagesInfo;
    VkPipelineRasterizationStateCreateInfo rasterizationInfo;
    VkPipelineMultisampleStateCreateInfo multisampleInfo;
    VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
    VkPipelineColorBlendStateCreateInfo colorBlendInfo;
    VkPipelineDynamicStateCreateInfo dynamicInfo;
  };

  SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface);

  VkShaderModule createShaderModule(VkDevice& device, std::string filepath);

  VkPipelineShaderStageCreateInfo createShaderStageInfo(VkShaderStageFlagBits shaderBit, VkShaderModule shaderModule);

  VkPipelineShaderStageCreateInfo CreateShaderStageInfo(VkShaderModule shaderModule, VkShaderStageFlagBits stage);

  VkPipelineVertexInputStateCreateInfo CreateVertexInputInfo();

  VkPipelineInputAssemblyStateCreateInfo CreateInputAssemblyInfo();

  VkPipelineViewportStateCreateInfo CreateViewportStateInfo();

  VkPipelineRasterizationStateCreateInfo CreateRasterizationStateInfo();

  VkPipelineMultisampleStateCreateInfo CreateMultisampleStateInfo();

  VkPipelineColorBlendAttachmentState CreateColorBlendAttachmentState();

  VkPipelineColorBlendStateCreateInfo
  CreateColorBlendStateInfo(const VkPipelineColorBlendAttachmentState *colorBlendAttachment);

  VkPipelineDynamicStateCreateInfo CreateDynamicStateInfo(const std::vector<VkDynamicState> &dynamicStates);
}
