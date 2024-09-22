#include "VulkanUtilities.hpp"
#include "../crow_utils.hpp"
#include "../Logger.hpp"

namespace crowe
{
  SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface){
    SwapChainSupportDetails details;

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

    if (formatCount != 0)
    {
      details.formats.resize(formatCount);
      vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
    }

    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);

    if (presentModeCount != 0)
    {
      details.presentModes.resize(presentModeCount);
      vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());
    }
    return details;
  }

  VkShaderModule CreateShaderModule(VkDevice device, std::string filepath)
  {
    VkShaderModule shaderModule;
    auto shaderCode = ReadFile(filepath);

    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = shaderCode.size();
    createInfo.pCode = reinterpret_cast<const uint32_t *>(shaderCode.data());

    if (vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS)
    {
      ERROR("Failed To Create Shader Module");
    }

    return shaderModule;
  }

  VkPipelineShaderStageCreateInfo CreateShaderStageInfo(VkShaderStageFlagBits shaderBit, VkShaderModule shaderModule)
  {
    VkPipelineShaderStageCreateInfo shaderStageCreateInfo{};
    shaderStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderStageCreateInfo.stage = shaderBit;
    shaderStageCreateInfo.module = shaderModule;
    shaderStageCreateInfo.pName = "main";
    return shaderStageCreateInfo;
  }

  VkPipelineShaderStageCreateInfo CreateShaderStageInfo(VkShaderModule shaderModule, VkShaderStageFlagBits stage) {
    VkPipelineShaderStageCreateInfo shaderStageInfo{};
    shaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderStageInfo.stage = stage;
    shaderStageInfo.module = shaderModule;
    shaderStageInfo.pName = "main";  // Entry point name (usually "main")
    return shaderStageInfo;
  }

// Function to create VkPipelineVertexInputStateCreateInfo
  VkPipelineVertexInputStateCreateInfo CreateVertexInputState(PipelineConfig& config) {
    VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputInfo.vertexBindingDescriptionCount = static_cast<uint32_t>(config.bindingDescriptions.size());
    vertexInputInfo.pVertexBindingDescriptions = config.bindingDescriptions.data();
    vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(config.attributeDescriptions.size());
    vertexInputInfo.pVertexAttributeDescriptions = config.attributeDescriptions.data();
    return vertexInputInfo;
  }

// Function to create VkPipelineInputAssemblyStateCreateInfo
  VkPipelineInputAssemblyStateCreateInfo CreateInputAssemblyState(PipelineConfig& config) {
    VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
    inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly.topology = config.topology;
    inputAssembly.primitiveRestartEnable = config.primitiveRestartEnable;
    return inputAssembly;
  }

// Function to create VkPipelineViewportStateCreateInfo
  VkPipelineViewportStateCreateInfo CreateViewportState(PipelineConfig& config) {
    VkPipelineViewportStateCreateInfo viewportState = {};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount = 1;
    viewportState.pViewports = &config.viewport;
    viewportState.scissorCount = 1;
    viewportState.pScissors = &config.scissor;
    return viewportState;
  }

// Function to create VkPipelineRasterizationStateCreateInfo
  VkPipelineRasterizationStateCreateInfo CreateRasterizationState(PipelineConfig& config) {
    VkPipelineRasterizationStateCreateInfo rasterizer = {};
    rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer.depthClampEnable = config.depthClampEnable;
    rasterizer.rasterizerDiscardEnable = config.rasterizerDiscardEnable;
    rasterizer.polygonMode = config.polygonMode;
    rasterizer.lineWidth = config.lineWidth;
    rasterizer.cullMode = config.cullMode;
    rasterizer.frontFace = config.frontFace;
    rasterizer.depthBiasEnable = config.depthBiasEnable;
    return rasterizer;
  }

// Function to create VkPipelineMultisampleStateCreateInfo
  VkPipelineMultisampleStateCreateInfo CreateMultisampleState(PipelineConfig& config) {
    VkPipelineMultisampleStateCreateInfo multisampling = {};
    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable = config.sampleShadingEnable;
    multisampling.rasterizationSamples = config.rasterizationSamples;
    return multisampling;
  }

// Function to create VkPipelineColorBlendAttachmentState
  VkPipelineColorBlendStateCreateInfo CreateColorBlendState(PipelineConfig& config) {
    VkPipelineColorBlendStateCreateInfo colorBlending = {};
    colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlending.logicOpEnable = config.logicOpEnable;
    colorBlending.logicOp = config.logicOp;
    colorBlending.attachmentCount = 1;
    colorBlending.pAttachments = &config.colorBlendAttachment;
    memcpy(colorBlending.blendConstants, config.blendConstants, sizeof(config.blendConstants));
    return colorBlending;
  }

// Function to create VkPipelineDepthStencilStateCreateInfo
  VkPipelineDepthStencilStateCreateInfo CreateDepthStencilState(PipelineConfig& config) {
    VkPipelineDepthStencilStateCreateInfo depthStencil = {};
    depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    depthStencil.depthTestEnable = config.depthTestEnable;
    depthStencil.depthWriteEnable = config.depthWriteEnable;
    depthStencil.depthCompareOp = config.depthCompareOp;
    depthStencil.depthBoundsTestEnable = config.depthBoundsTestEnable;
    depthStencil.stencilTestEnable = config.stencilTestEnable;
    return depthStencil;
  }

// Function to create VkPipelineDynamicStateCreateInfo
  VkPipelineDynamicStateCreateInfo CreateDynamicState(PipelineConfig& config) {
    VkPipelineDynamicStateCreateInfo dynamicState = {};
    dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicState.dynamicStateCount = static_cast<uint32_t>(config.dynamicStates.size());
    dynamicState.pDynamicStates = config.dynamicStates.data();
    return dynamicState;
  }

// Main function to create VkGraphicsPipelineCreateInfo
  VkGraphicsPipelineCreateInfo CreateGraphicsPipelineInfo(GraphicsPipelineCreateInfo graphicsPipelineCreateInfo) {
    VkGraphicsPipelineCreateInfo pipelineInfo = {};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount = 2; // Adjust this as necessary to match shader stages
    pipelineInfo.pStages = nullptr; // Set this to an actual array of shader stages

    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
    pipelineInfo.basePipelineIndex = -1;

    return pipelineInfo;
  }
}