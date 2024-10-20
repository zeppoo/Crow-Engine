#include "../../include/Vulkan/VulkanUtilities.hpp"
#include "crow_utils.hpp"
#include "Logger.hpp"

namespace crowe
{
  SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface)
  {
    SwapChainSupportDetails details;

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

    if (formatCount != 0) {
      details.formats.resize(formatCount);
      vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
    }

    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);

    if (presentModeCount != 0) {
      details.presentModes.resize(presentModeCount);
      vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());
    }
    return details;
  }

  PipelineInfo CreatePipelineInfo(PipelineSettings settings, VkExtent2D extent)
  {
    PipelineInfo pipelineInfo{};
    pipelineInfo.viewport = CreateViewPort(extent);
    pipelineInfo.scissor = CreateScissor(extent);
    pipelineInfo.vertexInputInfo = CreateVertexInputStateInfo(settings);
    pipelineInfo.inputAssemblyInfo = CreateInputAssemblyStateInfo(settings);
    pipelineInfo.tessellationInfo = CreateTessellationStateInfo(settings);
    pipelineInfo.viewportInfo = CreateViewportStateInfo(settings, &pipelineInfo.viewport,&pipelineInfo.scissor);
    pipelineInfo.rasterizationInfo = CreateRasterizationStateInfo(settings);
    pipelineInfo.multisampleInfo = CreateMultisampleStateInfo(settings);
    pipelineInfo.depthStencilInfo = CreateDepthStencilStateInfo(settings);
    pipelineInfo.colorBlendAttachmentState = CreateColorBlendAttachmentState(settings);
    pipelineInfo.colorBlendInfo = CreateColorBlendStateInfo(settings,&pipelineInfo.colorBlendAttachmentState);
    pipelineInfo.dynamicStatesInfo = CreateDynamicStateInfo(settings);
    return pipelineInfo;
  }

  VkShaderModule CreateShaderModule(VkDevice device, const std::string &filepath)
  {
    VkShaderModule shaderModule;
    auto shaderCode = ReadFile(filepath);

    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = shaderCode.size();
    createInfo.pCode = reinterpret_cast<const uint32_t *>(shaderCode.data());

    if (vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
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

  VkPipelineVertexInputStateCreateInfo CreateVertexInputStateInfo(PipelineSettings &settings)
  {
    VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputInfo.vertexBindingDescriptionCount = 0;
    vertexInputInfo.vertexAttributeDescriptionCount = 0;
    return vertexInputInfo;
  }

  VkPipelineInputAssemblyStateCreateInfo CreateInputAssemblyStateInfo(PipelineSettings &settings)
  {
    VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
    inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly.topology = settings.topology;
    inputAssembly.primitiveRestartEnable = settings.primitiveRestartEnable;
    return inputAssembly;
  }

  VkPipelineTessellationStateCreateInfo CreateTessellationStateInfo(PipelineSettings &settings)
  {
    VkPipelineTessellationStateCreateInfo tessellationInfo{};
    tessellationInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO;
    tessellationInfo.patchControlPoints = settings.patchControlPoints;
    return tessellationInfo;
  }

  VkViewport CreateViewPort(VkExtent2D extent)
  {
    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = (float) extent.width;
    viewport.height = (float) extent.height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    return viewport;
  }

  VkRect2D CreateScissor(VkExtent2D extent)
  {
    VkRect2D scissor{};
    scissor.offset = {0, 0};
    scissor.extent = extent;
    return scissor;
  }

  VkPipelineViewportStateCreateInfo CreateViewportStateInfo(PipelineSettings &settings, VkViewport *viewport, VkRect2D *scissor)
  {
    VkPipelineViewportStateCreateInfo viewportState{};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount = settings.viewportCount;
    viewportState.pViewports = viewport;
    viewportState.scissorCount = settings.scissorCount;
    viewportState.pScissors = scissor;
    return viewportState;
  }

  VkPipelineRasterizationStateCreateInfo CreateRasterizationStateInfo(PipelineSettings &settings)
  {
    VkPipelineRasterizationStateCreateInfo rasterizer{};
    rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer.depthClampEnable = settings.depthClampEnable;
    rasterizer.rasterizerDiscardEnable = settings.rasterizerDiscardEnable;
    rasterizer.polygonMode = settings.polygonMode;
    rasterizer.lineWidth = settings.lineWidth;
    rasterizer.cullMode = settings.cullMode;
    rasterizer.frontFace = settings.frontFace;
    rasterizer.depthBiasEnable = settings.depthBiasEnable;
    return rasterizer;
  }

  VkPipelineMultisampleStateCreateInfo CreateMultisampleStateInfo(PipelineSettings &settings)
  {
    VkPipelineMultisampleStateCreateInfo multisampling{};
    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable = settings.sampleShadingEnable;
    multisampling.rasterizationSamples = settings.rasterizationSamples;
    return multisampling;
  }

  VkPipelineDepthStencilStateCreateInfo CreateDepthStencilStateInfo(PipelineSettings &settings)
  {
    VkPipelineDepthStencilStateCreateInfo depthStencilInfo = {};
    depthStencilInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO; // Set the structure type
    // Depth testing settings
    depthStencilInfo.depthTestEnable = settings.depthTestEnable;           // Enable depth testing
    depthStencilInfo.depthWriteEnable = settings.depthWriteEnable;          // Allow writing to the depth buffer
    depthStencilInfo.depthCompareOp = settings.depthCompareOp; // Specifies the depth comparison operation (e.g., VK_COMPARE_OP_LESS means closer fragments pass)
    // Depth bounds test (optional)
    depthStencilInfo.depthBoundsTestEnable = settings.depthBoundsTestEnable;
    depthStencilInfo.minDepthBounds = 0.0f;
    depthStencilInfo.maxDepthBounds = 1.0f;
    // Stencil settings (optional)
    depthStencilInfo.stencilTestEnable = settings.stencilTestEnable;
    depthStencilInfo.front = {};
    depthStencilInfo.back = {};
    return depthStencilInfo;
  }

  VkPipelineColorBlendAttachmentState CreateColorBlendAttachmentState(PipelineSettings &settings)
  {
    VkPipelineColorBlendAttachmentState colorBlendAttachment{};
    colorBlendAttachment.colorWriteMask = settings.colorWriteMask;
    colorBlendAttachment.blendEnable = settings.blendEnable;
    colorBlendAttachment.srcColorBlendFactor = settings.srcColorBlendFactor;
    colorBlendAttachment.dstColorBlendFactor = settings.dstColorBlendFactor;
    colorBlendAttachment.colorBlendOp = settings.colorBlendOp;
    colorBlendAttachment.srcAlphaBlendFactor = settings.srcAlphaBlendFactor;
    colorBlendAttachment.dstAlphaBlendFactor = settings.dstAlphaBlendFactor;
    colorBlendAttachment.alphaBlendOp = settings.alphaBlendOp;
    return colorBlendAttachment;
  }


  VkPipelineColorBlendStateCreateInfo
  CreateColorBlendStateInfo(PipelineSettings &settings, const VkPipelineColorBlendAttachmentState *colorBlendAttachment)
  {
    VkPipelineColorBlendStateCreateInfo colorBlending{};
    colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlending.logicOpEnable = settings.logicOpEnable;
    colorBlending.logicOp = settings.logicOp;
    colorBlending.attachmentCount = settings.attachmentCount;
    colorBlending.pAttachments = colorBlendAttachment;
    colorBlending.blendConstants[0] = 0.0f;
    colorBlending.blendConstants[1] = 0.0f;
    colorBlending.blendConstants[2] = 0.0f;
    colorBlending.blendConstants[3] = 0.0f;
    return colorBlending;
  }

  VkPipelineDynamicStateCreateInfo CreateDynamicStateInfo(PipelineSettings &settings)
  {
    VkPipelineDynamicStateCreateInfo dynamicState{};
    dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicState.dynamicStateCount = static_cast<uint32_t>(settings.dynamicStates.size());
    dynamicState.pDynamicStates = settings.dynamicStates.data();
    return dynamicState;
  }
}