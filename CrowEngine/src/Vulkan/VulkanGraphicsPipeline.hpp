#pragma once

#include "../crow_lib.hpp"
#include "VulkanDevice.hpp"
#include "VulkanSwapChain.hpp"
#include "VulkanUtilities.hpp"

namespace crowe
{


  class VulkanGraphicsPipeline {
  public:
    VulkanGraphicsPipeline(std::unique_ptr<VulkanDevice>& device, std::unique_ptr<VulkanSwapChain>& swapchain);

    ~VulkanGraphicsPipeline();

    VkPipelineLayout getPipelineLayout() { return pipelineLayout; }
    VkPipeline getGraphicsPipeline() { return graphicsPipeline; }

  private:
    void createGraphicsPipeline(GraphicsPipelineCreateInfo graphicsPipelineCreateInfo);
    void createGraphicsPipelineInfo();
    void createPipelineLayout();
    void createDescriptorSetLayout();
    void createDescriptorPool();
    void createDescriptorSets();

    std::unique_ptr<VulkanDevice>& device;
    std::unique_ptr<VulkanSwapChain>& swapchain;
    VkPipelineLayout pipelineLayout;
    VkPipeline graphicsPipeline;
    VkDescriptorSetLayout descriptorSetLayout;
    VkDescriptorPool descriptorPool;
    std::vector<VkDescriptorSet> descriptorSets;
  };
}
