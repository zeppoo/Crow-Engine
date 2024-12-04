#pragma once

#include "crow_lib.hpp"
#include "Vulkan/Device.hpp"

namespace vulkan
{
  const uint8_t FRAMES_IN_FLIGHT = 3;

  struct Frame
  {
    VkImage image;
    VkImageView imageView;
    VkFramebuffer buffer;
    VkCommandBuffer commandBuffer;
    VkSemaphore semaphore[2];
    VkFence fence;
  };

  class FrameManager
  {
  public:
    FrameManager(std::unique_ptr<Device> &device, std::unique_ptr<QueueManager> &queueManager);

    std::vector<Frame> GetFrames(){ return frames; }
    uint32_t GetCurrentFrame(){return currentFrame; }

    void DestroyFrames();
    void CreateSwapchainImages(VkSwapchainKHR& swapchain, uint32_t imageCount);
    void CreateImageViews(VkFormat swapchainImageFormat);
    void CreateFrameBuffers(VkExtent2D swapchainExtent, VkRenderPass renderPass);
    void CreateSynchronizationObjects();
    VkCommandBuffer* BeginCommandBuffer(VkSwapchainKHR swapchain);
    void EndCommandBuffer(VkSwapchainKHR swapchain);

  private:
    std::unique_ptr<QueueManager> &queueManager;
    std::unique_ptr<Device> &device;
    VkDescriptorPool descriptorPool;
    std::vector<Frame> frames;
    uint32_t currentFrame = 0;
  };
}