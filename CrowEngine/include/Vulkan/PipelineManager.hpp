#pragma once

#include "crow_lib.hpp"
#include "Device.hpp"
#include "SwapChain.hpp"
#include "PipelineInfo.hpp"
#include "FrameManager.hpp"

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
    PipelineManager(std::unique_ptr<Device> &device, std::unique_ptr<SwapChain> &swapChain, std::unique_ptr<FrameManager> &frameManager);

    std::vector<GraphicsPipeline> GetGraphicsPipelines() {return graphicsPipelines;}

    void SetupPipelineManager();

    void CreateGraphicsPipeline(PipelineInfo pipelineInfo);

    void BindPipeline(VkCommandBuffer* pCommandBuffer, GraphicsPipeline& graphicsPipeline);

  private:

    VkDescriptorSetLayout CreateDescriptorSetLayout();

    void createDescriptorPool();
    void createDescriptorSets();

    std::unique_ptr<Device> &device;
    std::unique_ptr<SwapChain> &swapchain;
    std::unique_ptr<FrameManager> &frameManager;
    std::vector<GraphicsPipeline> graphicsPipelines;
    VkDescriptorPool descriptorPool;
    std::vector<VkDescriptorSet> descriptorSets;
  };
}