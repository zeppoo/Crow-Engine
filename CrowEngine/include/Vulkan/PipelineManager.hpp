#pragma once

#include "crow_lib.hpp"
#include "Device.hpp"
#include "SwapChain.hpp"
#include "PipelineInfo.hpp"

namespace vulkan{

  struct GraphicsPipeline
  {
    VkPipeline pipeline;
    VkPipelineLayout pipelineLayout;
    VkDescriptorSetLayout descriptorSetLayout;

    void InitializePipelineLayout(const VkDevice &device);
    void InitializePipeline(const VkDevice& device, const VkRenderPass& renderPass, const PipelineInfo pipelineInfo);
  };

  class PipelineManager
  {
  public:
    PipelineManager(std::unique_ptr<Device> &device, std::unique_ptr<SwapChain> &swapChain);

    void SetupPipelineManager();

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