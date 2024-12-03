#pragma once

#include "crow_lib.hpp"
#include "Vulkan/Device.hpp"

namespace vulkan
{
  const uint8_t FRAMES_IN_FLIGHT = 2;

  struct Frame
  {
    VkImage image;
    VkImageView ImageView;
    VkFramebuffer buffer;
    VkSemaphore semaphore;
    VkFence fence;
  };

  class FrameManager
  {
  public:
    FrameManager(std::unique_ptr<Device> &device);

  private:
    void FrameManagerInitialization();
    void CreateImageView(VkImage& image, VkImageView& imageView, VkFormat swapchainImageFormat);
    void CreateFrameBuffer(VkFramebuffer& framebuffer, VkImageView& imageView, VkExtent2D swapchainExtent, VkRenderPass renderPass);

    std::unique_ptr<Device> &device;

    VkDescriptorPool descriptorPool;
    std::vector<Frame> frames;
  };
}