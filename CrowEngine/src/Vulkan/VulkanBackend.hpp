#pragma once

#include "../crow_lib.hpp"
#include "../Core/Window.hpp"
#include "../Logger.hpp"
#include "VulkanQueueManager.hpp"
#include "VulkanDevice.hpp"
#include "VulkanSwapChain.hpp"
#include "VulkanGraphicsPipeline.hpp"
#include "VulkanDebugger.hpp"

namespace crowe{

  class VulkanModule
  {
  public:
    VulkanModule(std::unique_ptr<Window>& window);
    ~VulkanModule();

    void VulkanStartup();
    void CheckPhysicalDevice();
    void RecreateLogicalDevice();
    void RecreateGraphicsPipeline();
    void RecreateSwapchain();
  private:
    // References
    Logger& logger = Logger::GetInstance();
    std::unique_ptr<Window>& window;
    // Memebers
    std::unique_ptr<VulkanQueueManager> vulkanQueueManager;
    std::unique_ptr<VulkanDevice> device;
    std::unique_ptr<VulkanSwapChain> swapchain;
  };
}
