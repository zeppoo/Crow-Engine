#include "Vulkan/PipelineInfo.hpp"
#include "Vulkan/VulkanUtilities.hpp"
#include "Utils/FileUtilities.hpp"
#include "Logger.hpp"
#include "ConfigPaths.hpp"

namespace vulkan
{
  PipelineInfo CreatePipelineInfo(PipelineSettings &settings, VkExtent2D extent)
  {
    PipelineInfo pipelineInfo{};
    pipelineInfo.viewport = CreateViewPort(extent);
    pipelineInfo.scissor = CreateScissor(extent);
    pipelineInfo.bindingDescription = CreateBindingDescription();
    pipelineInfo.attributeDescriptions = CreateAttributeDescriptions();
    pipelineInfo.vertexInputInfo = CreateVertexInputStateInfo(settings, pipelineInfo.bindingDescription, pipelineInfo.attributeDescriptions);
    pipelineInfo.inputAssemblyInfo = CreateInputAssemblyStateInfo(settings);
    pipelineInfo.tessellationInfo = CreateTessellationStateInfo(settings);
    pipelineInfo.viewportInfo = CreateViewportStateInfo(settings, &pipelineInfo.viewport, &pipelineInfo.scissor);
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
      logger::Error("Failed To Create Shader Module");
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

  VkVertexInputBindingDescription CreateBindingDescription() {
      VkVertexInputBindingDescription bindingDescription{};

      bindingDescription.binding = 0;
      bindingDescription.stride = sizeof(Vertex);
      bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

      return bindingDescription;
  }

  std::array<VkVertexInputAttributeDescription, 2> CreateAttributeDescriptions() {
      std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions{};

      attributeDescriptions[0].location = 0;
      attributeDescriptions[0].binding = 0;
      attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
      attributeDescriptions[0].offset = offsetof(Vertex, pos);

      attributeDescriptions[1].location = 1;
      attributeDescriptions[1].binding = 0;
      attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
      attributeDescriptions[1].offset = offsetof(Vertex, color);

      return attributeDescriptions;
  }

  VkPipelineVertexInputStateCreateInfo CreateVertexInputStateInfo(PipelineSettings &settings, VkVertexInputBindingDescription &bindingDescription, std::array<VkVertexInputAttributeDescription, 2> &attributeDescriptions)
  {
    VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputInfo.vertexBindingDescriptionCount = 1;
    vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
    vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
    vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();
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