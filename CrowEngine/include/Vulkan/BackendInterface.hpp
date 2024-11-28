#pragma once

#include "crow_lib.hpp"
#include "../Core/Window.hpp"
#include "QueueManager.hpp"
#include "Device.hpp"
#include "SwapChain.hpp"
#include "PipelineManager.hpp"
#include "PipelineInfo.hpp"
#include "Debugger.hpp"
#include "VulkanUtilities.hpp"

namespace vulkan
{

  class VulkanModule {
  public:
    explicit VulkanModule(std::unique_ptr<core::Window> &window);

    ~VulkanModule();

    bool Startup();

    void ShutDown();

    void RenderFrame();

    void CreateNewGraphicsPipeline();
    void CreateNewGraphicsPipeline(const char* pipelineConfigFile);

    void RecreateSwapchain();

    void RecreateGraphicsPipeline();

  private:
    // References
    std::unique_ptr<core::Window> &window;
    // Memebers
    std::unique_ptr<QueueManager> queueManager;
    std::unique_ptr<Device> device;
    std::unique_ptr<SwapChain> swapchain;
    std::unique_ptr<PipelineManager> pipelineManager;
  };
}
