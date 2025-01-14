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
    explicit VulkanModule(std::shared_ptr<core::Window> &window);

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
    std::shared_ptr<core::Window> &window;
    // Memebers
    std::shared_ptr<QueueManager> queueManager;
    std::shared_ptr<Device> device;
    std::shared_ptr<FrameManager> frameManager;
    std::shared_ptr<SwapChain> swapchain;
    std::shared_ptr<PipelineManager> pipelineManager;
    std::shared_ptr<BufferManager> bufferManager;
    std::shared_ptr<CommandBufferManager> cmdBufferManager;
  };
}
