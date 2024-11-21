#pragma once

#include "crow_lib.hpp"

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
    FrameManager();

  private:
    VkDescriptorPool descriptorPool;

    std::vector<Frame> frames;
  };
}