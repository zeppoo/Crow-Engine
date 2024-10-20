#pragma once

#include "crow_lib.hpp"
#include "VulkanDevice.hpp"
#include "VulkanSwapChain.hpp"
#include "Vulkan/VulkanUtilities.hpp"

namespace crowe
{
  class VulkanGraphicsPipeline {
  public:
    VulkanGraphicsPipeline(std::unique_ptr<VulkanDevice> &device, std::unique_ptr<VulkanSwapChain> &swapChain);

    ~VulkanGraphicsPipeline();

    void CreateGraphicsPipeline(PipelineInfo pipelineInfo);

    VkPipelineLayout getPipelineLayout()
    { return pipelineLayout; }

    VkPipeline getGraphicsPipeline()
    { return graphicsPipeline; }

  private:

    void CreatePipelineLayout();

    void createDescriptorSetLayout();

    void createDescriptorPool();

    void createDescriptorSets();

    std::unique_ptr<VulkanDevice> &device;
    std::unique_ptr<VulkanSwapChain> &swapchain;
    VkPipelineLayout pipelineLayout;
    VkPipeline graphicsPipeline;
    VkDescriptorSetLayout descriptorSetLayout;
    VkDescriptorPool descriptorPool;
    std::vector<VkDescriptorSet> descriptorSets;
  };
}
