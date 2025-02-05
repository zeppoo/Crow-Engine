#include "Vulkan/FrameManager.hpp"
#include "Utils/Logger.hpp"

namespace vulkan
{
  FrameManager::FrameManager(std::shared_ptr<Device> device, std::shared_ptr<QueueManager> queueManager, std::shared_ptr<SwapChain> swapchain) : device{device}, queueManager{queueManager}, swapchain{swapchain} {}

  void FrameManager::CreateFrameSyncObjects()
  {
    for (Frame & frame : frames) {
      VkSemaphoreCreateInfo semaphoreInfo{};
      semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

      VkFenceCreateInfo fenceInfo{};
      fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
      fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

      if (vkCreateSemaphore(device->GetDevice(), &semaphoreInfo, nullptr, &frame.semaphore[0]) != VK_SUCCESS ||
        vkCreateSemaphore(device->GetDevice(), &semaphoreInfo, nullptr, &frame.semaphore[1]) != VK_SUCCESS ||
        vkCreateFence(device->GetDevice(), &fenceInfo, nullptr, &frame.fence) != VK_SUCCESS) {
        throw std::runtime_error("failed to create synchronization objects!");
      }
    }
  }

  void FrameManager::PrepareFrame(VkCommandBuffer* pCommandBuffer, VkSwapchainKHR swapchain)
  {
    uint32_t imageIndex;
    vkWaitForFences(device->GetDevice(), 1, &frames[currentFrame].fence, VK_TRUE, UINT64_MAX);
    vkResetFences(device->GetDevice(), 1, &frames[currentFrame].fence);
    vkAcquireNextImageKHR(device->GetDevice(), swapchain, UINT64_MAX, frames[currentFrame].semaphore[0], VK_NULL_HANDLE, &imageIndex);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphores[] = {frames[currentFrame].semaphore[0]};
    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;

    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = pCommandBuffer;

    VkSemaphore signalSemaphores[] = {frames[currentFrame].semaphore[1]};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    if (vkQueueSubmit(*queueManager->GetGraphicsQueues()[0].pQueue, 1, &submitInfo, frames[currentFrame].fence) != VK_SUCCESS) {
      throw std::runtime_error("failed to submit draw command buffer!");
    }
  }

  void FrameManager::PresentFrame(VkSwapchainKHR swapchain)
  {
    queueManager->GetPresentQueues()[0].SubmitBuffers(device->GetDevice());

    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    VkSemaphore RenderFinishSemaphores[] = {frames[currentFrame].semaphore[1]};
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = RenderFinishSemaphores;

    VkSwapchainKHR swapChains[] = {swapchain};
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;

    presentInfo.pImageIndices = &currentImage;

    vkQueuePresentKHR(*queueManager->GetPresentQueues()[0].pQueue, &presentInfo);

    currentFrame = (currentFrame + 1) % FRAMES_IN_FLIGHT;
  }
}