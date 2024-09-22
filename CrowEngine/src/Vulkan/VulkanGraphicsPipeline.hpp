#pragma once

#include "../crow_lib.hpp"
#include "VulkanDevice.hpp"
#include "VulkanSwapChain.hpp"

namespace crowe
{


  class VulkanGraphicsPipeline {
  public:
    VulkanGraphicsPipeline(VulkanDevice &device, VulkanSwapChain &swapchain);

    ~VulkanGraphicsPipeline();

    VkPipelineLayout getPipelineLayout() { return pipelineLayout; }
    VkPipeline getGraphicsPipeline() { return graphicsPipeline; }

  private:
    void createGraphicsPipeline();
    void createGraphicsPipelineInfo();
    void createPipelineLayout();
    void createDescriptorSetLayout();
    void createDescriptorPool();
    void createDescriptorSets();

    VulkanDevice &device;
    VulkanSwapChain &swapchain;
    VkPipelineLayout pipelineLayout;
    VkPipeline graphicsPipeline;
    VkDescriptorSetLayout descriptorSetLayout;
    VkDescriptorPool descriptorPool;
    std::vector<VkDescriptorSet> descriptorSets;
  };
}
