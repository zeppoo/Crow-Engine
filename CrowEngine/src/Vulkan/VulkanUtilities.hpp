#pragma once

#include "../crow_lib.hpp"


namespace crowe
{
  struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
  };

  struct PipelineConfig{
    // Vertex Input State
    std::vector<VkVertexInputBindingDescription> bindingDescriptions;
    std::vector<VkVertexInputAttributeDescription> attributeDescriptions;

    // Input Assembly State
    VkPrimitiveTopology topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    VkBool32 primitiveRestartEnable = VK_FALSE;

    // Viewport and Scissor
    VkViewport viewport;
    VkRect2D scissor;

    // Rasterization State
    VkBool32 depthClampEnable = VK_FALSE;
    VkBool32 rasterizerDiscardEnable = VK_FALSE;
    VkPolygonMode polygonMode = VK_POLYGON_MODE_FILL;
    VkCullModeFlags cullMode = VK_CULL_MODE_BACK_BIT;
    VkFrontFace frontFace = VK_FRONT_FACE_CLOCKWISE;
    VkBool32 depthBiasEnable = VK_FALSE;
    float lineWidth = 1.0f;

    // Multisample State
    VkSampleCountFlagBits rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    VkBool32 sampleShadingEnable = VK_FALSE;

    // Color Blend State
    VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
    VkBool32 logicOpEnable = VK_FALSE;
    VkLogicOp logicOp = VK_LOGIC_OP_COPY;
    float blendConstants[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

    // Depth Stencil State
    VkBool32 depthTestEnable = VK_TRUE;
    VkBool32 depthWriteEnable = VK_TRUE;
    VkCompareOp depthCompareOp = VK_COMPARE_OP_LESS;
    VkBool32 depthBoundsTestEnable = VK_FALSE;
    VkBool32 stencilTestEnable = VK_FALSE;

    // Dynamic States
    std::vector<VkDynamicState> dynamicStates;

    // Other states (not directly in the pipeline struct)
    VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;
    VkRenderPass renderPass = VK_NULL_HANDLE;
    uint32_t subpass = 0;
  };

  struct GraphicsPipelineCreateInfo {
    VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
    VkPipelineTessellationStateCreateInfo tessellationInfo;
    VkPipelineVertexInputStateCreateInfo vertexInputInfo;
    VkPipelineViewportStateCreateInfo viewportInfo;
    VkPipelineShaderStageCreateInfo shaderStagesInfo;
    VkPipelineRasterizationStateCreateInfo rasterizationInfo;
    VkPipelineMultisampleStateCreateInfo multisampleInfo;
    VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
    VkPipelineColorBlendAttachmentState colorBlendAttachment;
    VkPipelineColorBlendStateCreateInfo colorBlendInfo;
    VkPipelineDynamicStateCreateInfo dynamicInfo;
  };

  SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface);

  VkShaderModule CreateShaderModule(VkDevice device, std::string filepath);

  VkPipelineShaderStageCreateInfo CreateShaderStageInfo(VkShaderStageFlagBits shaderBit, VkShaderModule shaderModule);

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
