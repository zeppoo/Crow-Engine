#pragma once

#include "crow_lib.hpp"
#include "Device.hpp"
#include "QueueManager.hpp"
#include "FrameManager.hpp"
#include "Config/RenderPassSettings.hpp"

namespace vulkan
{
  class SwapChain {
  public:
    SwapChain(std::unique_ptr<Device> &device, std::unique_ptr<QueueManager> &queueManager, std::unique_ptr<FrameManager> &frameManager);

    ~SwapChain();

    void SetupSwapChain();

    void RecreateSwapChain();

    VkSwapchainKHR GetSwapchain()
    { return swapchain; }

    VkRenderPass GetRenderPass()
    { return renderPass; }

    VkFormat GetSwapchainImageFormat()
    { return swapchainImageFormat; }

    VkExtent2D GetSwapchainExtent()
    { return swapchainExtent; }

  private:
    void CleanupSwapChain();

    void createSwapChain(VkSwapchainKHR oldSwapChain);

    void CreateRenderPass(RenderPassConfig& RenderPassConfig);

    void createImageViews();

    void createFramebuffers();

    VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats);
    VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes);
    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities);

    std::unique_ptr<Device> &device;
    std::unique_ptr<QueueManager> &queueManager;
    std::unique_ptr<FrameManager> &frameManager;
    VkSwapchainKHR swapchain;
    VkRenderPass renderPass;
    VkFormat swapchainImageFormat;
    VkExtent2D swapchainExtent;
  };

}
