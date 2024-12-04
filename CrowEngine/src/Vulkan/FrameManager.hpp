#pragma once

#include "crow_lib.hpp"
#include "Vulkan/Device.hpp"

namespace vulkan
{
  const uint8_t FRAMES_IN_FLIGHT = 2;

  struct Frame
  {
    VkImage image;
    VkImageView imageView;
    VkFramebuffer buffer;
    VkCommandBuffer commandBuffer;
    VkSemaphore semaphore;
    VkFence fence;
  };

  class FrameManager
  {
  public:
    FrameManager(std::unique_ptr<Device> &device);

    void CreateImageViews(VkFormat swapchainImageFormat);
    void CreateFrameBuffers(VkExtent2D swapchainExtent, VkRenderPass renderPass);
    void AllocateCommandBuffers();
    void RecordCommandBuffer();

  private:
    void FrameManagerInitialization();


    std::unique_ptr<Device> &device;

    VkDescriptorPool descriptorPool;
    std::vector<Frame> frames;
  };
}