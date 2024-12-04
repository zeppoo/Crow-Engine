#include "Vulkan/FrameManager.hpp"
#include "Logger.hpp"

namespace vulkan
{
  FrameManager::FrameManager(std::unique_ptr<Device> &device, std::unique_ptr<QueueManager> &queueManager) : device{device}, queueManager{queueManager} {}

  void FrameManager::DestroyFrames()
  {
    for(Frame & frame : frames) {
      vkDestroyFramebuffer(device->getDevice(), frame.buffer, nullptr);
      vkDestroyImageView(device->getDevice(), frame.imageView, nullptr);
    }
  }

  void FrameManager::CreateSwapchainImages(VkSwapchainKHR& swapchain, uint32_t imageCount)
  {
    std::vector<VkImage> swapchainImages;
    // Step 1: Get the number of swapchain images
    vkGetSwapchainImagesKHR(device->getDevice(), swapchain, &imageCount, nullptr);
    swapchainImages.resize(imageCount);
    // Step 2: Retrieve the swapchain images
    vkGetSwapchainImagesKHR(device->getDevice(), swapchain, &imageCount, swapchainImages.data());

    // Step 3: Create Frame structs for each swapchain image
    frames.resize(imageCount);

    for (size_t i = 0; i < imageCount; i++) {
      frames[i].image = swapchainImages[i];
    }

    CreateSynchronizationObjects();
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

  void FrameManager::CreateSynchronizationObjects()
  {
    for (Frame & frame : frames) {
      VkSemaphoreCreateInfo semaphoreInfo{};
      semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

      VkFenceCreateInfo fenceInfo{};
      fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
      fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

      if (vkCreateSemaphore(device->getDevice(), &semaphoreInfo, nullptr, &frame.semaphore[0]) != VK_SUCCESS ||
        vkCreateSemaphore(device->getDevice(), &semaphoreInfo, nullptr, &frame.semaphore[1]) != VK_SUCCESS ||
        vkCreateFence(device->getDevice(), &fenceInfo, nullptr, &frame.fence) != VK_SUCCESS) {
        throw std::runtime_error("failed to create semaphores!");
      }
    }
  }

  VkCommandBuffer* FrameManager::BeginCommandBuffer(VkSwapchainKHR swapchain)
  {
    vkWaitForFences(device->getDevice(), 1, &frames[currentFrame].fence, VK_TRUE, UINT64_MAX);
    vkResetFences(device->getDevice(), 1, &frames[currentFrame].fence);

    vkAcquireNextImageKHR(device->getDevice(), swapchain, UINT64_MAX, frames[currentFrame].semaphore[0], VK_NULL_HANDLE, &currentFrame);
    vkResetCommandBuffer(frames[currentFrame].commandBuffer, /*VkCommandBufferResetFlagBits*/ 0);

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = 0; // Optional
    beginInfo.pInheritanceInfo = nullptr; // Optional

    if (vkBeginCommandBuffer(frames[currentFrame].commandBuffer, &beginInfo) != VK_SUCCESS) {
      throw std::runtime_error("failed to begin recording command buffer!");
    }

    return &frames[currentFrame].commandBuffer;
  }

void FrameManager::EndCommandBuffer(VkSwapchainKHR swapchain)
  {
    if (vkEndCommandBuffer(frames[currentFrame].commandBuffer) != VK_SUCCESS) {
      throw std::runtime_error("failed to record command buffer!");
    }

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphores[] = {frames[currentFrame].semaphore[0]};
    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;

    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &frames[currentFrame].commandBuffer;

    VkSemaphore signalSemaphores[] = {frames[currentFrame].semaphore[1]};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    if (vkQueueSubmit(*queueManager->GetGraphicsQueues()[0].pQueue, 1, &submitInfo, frames[currentFrame].fence) != VK_SUCCESS) {
      throw std::runtime_error("failed to submit draw command buffer!");
    }

    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    VkSwapchainKHR swapChains[] = {swapchain};
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;

    presentInfo.pImageIndices = &currentFrame;

    vkQueuePresentKHR(*queueManager->GetPresentQueues()[0].pQueue, &presentInfo);

    currentFrame = (currentFrame + 1) % FRAMES_IN_FLIGHT;
  }
}