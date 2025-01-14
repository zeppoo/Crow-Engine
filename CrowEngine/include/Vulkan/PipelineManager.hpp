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
    VkDescriptorPool descriptorPool;
    std::vector<VkDescriptorSet> descriptorSets;

    void InitializePipelineLayout(const VkDevice &device);
    void InitializePipeline(const VkDevice& device, const VkRenderPass& renderPass, const PipelineInfo pipelineInfo);
    void InitializeDescriptorLayout(const VkDevice& device);
    void InitializeDescriptorPool(const VkDevice& device, std::vector<VkDescriptorPoolSize> poolSizes);
    void InitializeDescriptorSets(const VkDevice& device, std::vector<Buffer> buffers);
    void BindPipeline(VkCommandBuffer* pCommandBuffer);
  };

  class PipelineManager
  {
  public:
    PipelineManager(std::shared_ptr<Device> device, std::shared_ptr<SwapChain> swapChain, std::shared_ptr<FrameManager> frameManager);

    std::vector<GraphicsPipeline> GetGraphicsPipelines() {return graphicsPipelines;}

    void SetupPipelineManager();

    void CreateGraphicsPipeline(PipelineInfo pipelineInfo);


  private:

    VkDescriptorSetLayout CreateDescriptorSetLayout();

    void createDescriptorPool();
    void createDescriptorSets();

    std::shared_ptr<Device> &device;
    std::shared_ptr<SwapChain> &swapchain;
    std::shared_ptr<FrameManager> &frameManager;
    std::vector<GraphicsPipeline> graphicsPipelines;
    VkDescriptorPool descriptorPool;
    std::vector<VkDescriptorSet> descriptorSets;
  };
}