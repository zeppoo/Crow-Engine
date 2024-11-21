#pragma once

#include "crow_lib.hpp"
#include "Device.hpp"
#include "SwapChain.hpp"
#include "VulkanUtilities.hpp"

namespace vulkan{

  struct GraphicsPipeline
  {
    VkPipeline pipeline;
    VkPipelineLayout pipelineLayout;
    VkDescriptorSetLayout descriptorSetLayout;

    void InitializePipelineLayout(VkDevice& device);
    void InitializePipeline(VkDevice& device, VkRenderPass& renderPass, PipelineInfo pipelineInfo);
  };

  class PipelineManager
  {
  public:
    PipelineManager(std::unique_ptr<Device> &device, std::unique_ptr<SwapChain> &swapChain);

    void CreateGraphicsPipeline(PipelineInfo pipelineInfo);

  private:

    VkDescriptorSetLayout CreateDescriptorSetLayout();

    void createDescriptorPool();
    void createDescriptorSets();

    std::unique_ptr<Device> &device;
    std::unique_ptr<SwapChain> &swapchain;
    std::vector<GraphicsPipeline> graphicsPipelines;
    VkDescriptorPool descriptorPool;
    std::vector<VkDescriptorSet> descriptorSets;
  };
}