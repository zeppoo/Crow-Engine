#pragma once

#include "crow_lib.hpp"
#include "../Core/Window.hpp"
#include "VulkanQueueManager.hpp"
#include "VulkanDevice.hpp"
#include "VulkanSwapChain.hpp"
#include "VulkanGraphicsPipeline.hpp"
#include "VulkanDebugger.hpp"
#include "VulkanUtilities.hpp"


namespace crowe
{

  class VulkanModule {
  public:
    VulkanModule(std::unique_ptr<Window> &window);

    ~VulkanModule();

    bool VulkanStartup();

    void VulkanShutDown();

    void CreateNewGraphicsPipeline();
    void CreateNewGraphicsPipeline(const char* pipelineConfigFile);

    void RecreateSwapchain();

    void RecreateGraphicsPipeline(std::unique_ptr<VulkanGraphicsPipeline> graphicsPipeline);

  private:
    // References
    std::unique_ptr<Window> &window;
    // Memebers
    std::unique_ptr<VulkanQueueManager> queueManager;
    std::unique_ptr<VulkanDevice> device;
    std::unique_ptr<VulkanSwapChain> swapchain;
    std::vector<std::unique_ptr<VulkanGraphicsPipeline>> graphicsPipelines;
  };
}
