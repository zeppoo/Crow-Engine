#pragma once

#include "../crow_lib.hpp"
#include "VulkanQueueManager.hpp"
#include "VulkanDevice.hpp"
#include "VulkanSwapChain.hpp"
#include "VulkanGraphicsPipeline.hpp"
#include "VulkanDebugger.hpp"
#include "../Core/Window.hpp"

namespace crowe{

  class VulkanModule
  {
  public:
    VulkanModule();
    ~VulkanModule();

    void VulkanStartup();
    void CheckPhysicalDevice();
    void RecreateLogicalDevice();
    void RecreateGraphicsPipeline();
    void RecreateSwapchain();
  };
}
