#pragma once

#include "../Core/Window.hpp"
#include "BufferManager.hpp"
#include "Debugger.hpp"
#include "Device.hpp"
#include "FrameManager.hpp"
#include "PipelineInfo.hpp"
#include "PipelineManager.hpp"
#include "QueueManager.hpp"
#include "SwapChain.hpp"
#include "CommandBuffers.hpp"
#include "VulkanUtilities.hpp"
#include "crow_lib.hpp"

namespace vulkan
{
  class VulkanModule {
  public:
    explicit VulkanModule(std::unique_ptr<core::Window> &window);

    ~VulkanModule();

    QueueManager* GetQueueManager() const { return queueManager.get(); }
    Device* GetDevice() const { return device.get(); }
    FrameManager* GetFrameManager() const { return frameManager.get(); }
    SwapChain* GetSwapChain() const { return swapchain.get(); }
    PipelineManager* GetPipelineManager() const { return pipelineManager.get(); }
    BufferManager* GetBufferManager() const { return bufferManager.get(); }
    CommandBufferManager* GetCmdBufferManager() const { return cmdBufferManager.get(); }

    bool InitiliazeVulkan();

    void ShutDown();
    void RecordGUIBuffer();
    void RecordShaderBuffer();
    void BeginShaderExecution();
    void RenderingLoop();
    void EndShaderExecution();

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
    std::unique_ptr<FrameManager> frameManager;
    std::unique_ptr<SwapChain> swapchain;
    std::unique_ptr<PipelineManager> pipelineManager;
    std::unique_ptr<BufferManager> bufferManager;
    std::unique_ptr<CommandBufferManager> cmdBufferManager;
  };
}
