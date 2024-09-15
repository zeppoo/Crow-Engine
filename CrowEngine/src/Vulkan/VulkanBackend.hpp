#pragma once

#include "../crow_lib.hpp"
#include "../Core/Window.hpp"
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

    bool VulkanStartup();
    void VulkanShutDown();
    void RecreateSwapchain();
    void RecreateGraphicsPipeline();
  private:
    // References
    std::unique_ptr<Window>& window;
    // Memebers
    std::unique_ptr<VulkanQueueManager> queueManager;
    std::unique_ptr<VulkanDevice> device;
    std::unique_ptr<VulkanSwapChain> swapchain;
  };
}
