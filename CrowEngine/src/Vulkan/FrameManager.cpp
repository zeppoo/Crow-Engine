#include "FrameManager.hpp"
#include "Logger.hpp"

namespace vulkan
{
  FrameManager::FrameManager(std::unique_ptr<Device> &device) : device{device} {
    FrameManagerInitialization();
  }

  void FrameManager::FrameManagerInitialization()
  {
    for (int i = 0; i < FRAMES_IN_FLIGHT; i++) {
      Frame newFrame{};
      frames.push_back(newFrame);
    }
  }

  void FrameManager::CreateImageViews(VkFormat swapchainImageFormat)
  {
    for (Frame & frame : frames)
    {
      VkImageViewCreateInfo createInfo{};
      createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
      createInfo.image = frame.image;
      createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
      createInfo.format = swapchainImageFormat;
      createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
      createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
      createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
      createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
      createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
      createInfo.subresourceRange.baseMipLevel = 0;
      createInfo.subresourceRange.levelCount = 1;
      createInfo.subresourceRange.baseArrayLayer = 0;
      createInfo.subresourceRange.layerCount = 1;

      if (vkCreateImageView(device->getDevice(), &createInfo, nullptr, &frame.imageView) != VK_SUCCESS) {
        logger::FatalError("Failed to create Image Views");
      }
      logger::Info("Created Image View");
    }
  }

  void FrameManager::CreateFrameBuffers(VkExtent2D swapchainExtent, VkRenderPass renderPass)
  {
    for (Frame & frame : frames)
    {
      VkImageView attachments[] = {frame.imageView};

      VkFramebufferCreateInfo framebufferInfo{};
      framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
      framebufferInfo.renderPass = renderPass;
      framebufferInfo.attachmentCount = 1;
      framebufferInfo.pAttachments = attachments;
      framebufferInfo.width = swapchainExtent.width;
      framebufferInfo.height = swapchainExtent.height;
      framebufferInfo.layers = 1;

      if (vkCreateFramebuffer(device->getDevice(), &framebufferInfo, nullptr, &frame.buffer) !=
          VK_SUCCESS) {
        throw std::runtime_error("failed to create framebuffer!");
      }
    }
  }

  void FrameManager::AllocateCommandBuffers()
  {

  }

  void FrameManager::RecordCommandBuffer()
  {}

}