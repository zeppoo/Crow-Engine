#pragma once

#include "crow_lib.hpp"
#include "glm/glm.hpp"
#include "../Config/PipelineSettings.hpp"
#include <array>

namespace vulkan
{
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

  struct Vertex {
    glm::vec2 pos;
    glm::vec3 color;

    static VkVertexInputBindingDescription getBindingDescription() {
      VkVertexInputBindingDescription bindingDescription{};

      bindingDescription.binding = 0; // Binding index in the shader
      bindingDescription.stride = sizeof(Vertex); // Total size of a vertex (position + color)
      bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX; // Per-vertex data

      return bindingDescription;
    }

    static std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions() {
      std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions{};
      // Position: Location = 0
      attributeDescriptions[0].location = 0; // Matches layout(location = 0) in the shader
      attributeDescriptions[0].binding = 0; // Matches binding index in the binding description
      attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT; // vec3 -> 3 floats
      attributeDescriptions[0].offset = offsetof(Vertex, pos); // Offset in the vertex struct

      // Color: Location = 1
      attributeDescriptions[1].location = 1; // Matches layout(location = 1) in the shader
      attributeDescriptions[1].binding = 0; // Same binding index
      attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT; // vec3 -> 3 floats
      attributeDescriptions[1].offset = offsetof(Vertex, color); // Offset in the vertex struct

      return attributeDescriptions;
    }
  };

  PipelineInfo* CreatePipelineInfo(PipelineSettings settings, VkExtent2D extent);

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
