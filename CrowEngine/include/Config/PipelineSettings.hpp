#pragma once
#include "crow_lib.hpp"
#include "cppr.hpp"

struct PipelineSettings {
  REFLECT();  // Add reflection capability

  // Vertex Input
  uint32_t vertexBindingDescriptionCount = 0;
  uint32_t vertexAttributeDescriptionCount = 0;

  // Input Assembly
  VkPrimitiveTopology topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
  VkBool32 primitiveRestartEnable = VK_FALSE;

  // Tessellation
  uint32_t patchControlPoints = 3;

  // Viewports and Scissors
  uint32_t viewportCount = 1;
  uint32_t scissorCount = 1;

  // Rasterizer
  VkBool32 depthClampEnable = VK_FALSE;
  VkBool32 rasterizerDiscardEnable = VK_FALSE;
  VkPolygonMode polygonMode = VK_POLYGON_MODE_FILL;
  VkCullModeFlags cullMode = VK_CULL_MODE_BACK_BIT;
  VkFrontFace frontFace = VK_FRONT_FACE_CLOCKWISE;
  VkBool32 depthBiasEnable = VK_FALSE;
  float depthBiasConstantFactor = 0.0f;
  float depthBiasClamp = 0.0f;
  float depthBiasSlopeFactor = 0.0f;
  float lineWidth = 1.0f;

  // Multisampling
  VkSampleCountFlagBits rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
  VkBool32 sampleShadingEnable = VK_FALSE;
  float minSampleShading = 1.0f;
  VkSampleMask sampleMask = 0;
  VkBool32 alphaToCoverageEnable = VK_FALSE;
  VkBool32 alphaToOneEnable = VK_FALSE;

  // Depth and Stencil Testing
  VkBool32 depthTestEnable = VK_TRUE;
  VkBool32 depthWriteEnable = VK_TRUE;
  VkCompareOp depthCompareOp = VK_COMPARE_OP_LESS;
  VkBool32 depthBoundsTestEnable = VK_FALSE;
  VkBool32 stencilTestEnable = VK_FALSE;

  // Color Blending Attachment
  VkBool32 blendEnable = VK_FALSE;
  VkBlendFactor srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
  VkBlendFactor dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
  VkBlendOp colorBlendOp = VK_BLEND_OP_ADD;
  VkBlendFactor srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
  VkBlendFactor dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
  VkBlendOp alphaBlendOp = VK_BLEND_OP_ADD;
  VkColorComponentFlags colorWriteMask =
      VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;

  // Color Blending
  VkBool32 logicOpEnable = VK_FALSE;
  VkLogicOp logicOp = VK_LOGIC_OP_COPY;
  uint32_t attachmentCount = 1;
  float blendConstants[4] = {0.0f, 0.0f, 0.0f, 0.0f};

  // Dynamic states
  std::vector<VkDynamicState> dynamicStates = {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR};
};

REFLECT_STRUCT_BEGIN(PipelineSettings)
  REFLECT_STRUCT_MEMBER(vertexBindingDescriptionCount)
  REFLECT_STRUCT_MEMBER(vertexAttributeDescriptionCount)
  REFLECT_STRUCT_MEMBER(topology)
  REFLECT_STRUCT_MEMBER(primitiveRestartEnable)
  REFLECT_STRUCT_MEMBER(patchControlPoints)
  REFLECT_STRUCT_MEMBER(viewportCount)
  REFLECT_STRUCT_MEMBER(scissorCount)
  REFLECT_STRUCT_MEMBER(depthClampEnable)
  REFLECT_STRUCT_MEMBER(rasterizerDiscardEnable)
  REFLECT_STRUCT_MEMBER(polygonMode)
  REFLECT_STRUCT_MEMBER(cullMode)
  REFLECT_STRUCT_MEMBER(frontFace)
  REFLECT_STRUCT_MEMBER(depthBiasEnable)
  REFLECT_STRUCT_MEMBER(depthBiasConstantFactor)
  REFLECT_STRUCT_MEMBER(depthBiasClamp)
  REFLECT_STRUCT_MEMBER(depthBiasSlopeFactor)
  REFLECT_STRUCT_MEMBER(lineWidth)
  REFLECT_STRUCT_MEMBER(rasterizationSamples)
  REFLECT_STRUCT_MEMBER(sampleShadingEnable)
  REFLECT_STRUCT_MEMBER(minSampleShading)
  REFLECT_STRUCT_MEMBER(sampleMask)
  REFLECT_STRUCT_MEMBER(alphaToCoverageEnable)
  REFLECT_STRUCT_MEMBER(alphaToOneEnable)
  REFLECT_STRUCT_MEMBER(depthTestEnable)
  REFLECT_STRUCT_MEMBER(depthWriteEnable)
  REFLECT_STRUCT_MEMBER(depthCompareOp)
  REFLECT_STRUCT_MEMBER(depthBoundsTestEnable)
  REFLECT_STRUCT_MEMBER(stencilTestEnable)
  REFLECT_STRUCT_MEMBER(blendEnable)
  REFLECT_STRUCT_MEMBER(srcColorBlendFactor)
  REFLECT_STRUCT_MEMBER(dstColorBlendFactor)
  REFLECT_STRUCT_MEMBER(colorBlendOp)
  REFLECT_STRUCT_MEMBER(srcAlphaBlendFactor)
  REFLECT_STRUCT_MEMBER(dstAlphaBlendFactor)
  REFLECT_STRUCT_MEMBER(alphaBlendOp)
  REFLECT_STRUCT_MEMBER(colorWriteMask)
  REFLECT_STRUCT_MEMBER(logicOpEnable)
  REFLECT_STRUCT_MEMBER(logicOp)
  REFLECT_STRUCT_MEMBER(attachmentCount)
  REFLECT_STRUCT_MEMBER(dynamicStates)
  REFLECT_STRUCT_MEMBER(blendConstants)
REFLECT_STRUCT_END()