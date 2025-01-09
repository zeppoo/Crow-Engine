#pragma once

#include "crow_lib.hpp"
#include "Vulkan/Device.hpp"
#include "Vulkan/SwapChain.hpp"
#include <Vulkan/BufferManager.hpp>

namespace vulkan
{
  const uint8_t FRAMES_IN_FLIGHT = 3;

  struct Frame
  {
    VkFramebuffer buffer;
    VkSemaphore semaphore[2];
    VkFence fence;
  };

  class FrameManager
  {
  public:
    FrameManager(std::unique_ptr<Device> &device, std::unique_ptr<QueueManager> &queueManager, std::unique_ptr<SwapChain> &swapchain);

    std::vector<Frame> GetFrames() { return frames; }
    uint32_t GetCurrentImage() { return currentImage; }
    uint32_t GetCurrentFrame() { return currentFrame; }

    void CreateFrameSyncObjects();
    void PrepareFrame(VkCommandBuffer* pCommandBuffer, VkSwapchainKHR swapchain);
    void PresentFrame(VkSwapchainKHR swapchain);

  private:
    std::unique_ptr<QueueManager> &queueManager;
    std::unique_ptr<Device> &device;
    std::unique_ptr<SwapChain> &swapchain;
    VkDescriptorPool descriptorPool;
    std::vector<Frame> frames;
    uint32_t currentImage;
    uint32_t currentFrame = 0;
  };
}