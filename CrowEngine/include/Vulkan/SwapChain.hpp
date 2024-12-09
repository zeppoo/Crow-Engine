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

    void CreateRenderPass(RenderPassConfig& RenderPassConfig);
    void BeginRenderPass(VkCommandBuffer* pCommandBuffer, VkRenderPass renderPass);
    void SetViewPort(VkCommandBuffer* pCommandBuffer);
    void SetScissor(VkCommandBuffer* pCommandBuffer);
    void EndRenderPass(VkCommandBuffer* pCommandBuffer);

    VkSwapchainKHR GetSwapchain()
    { return swapchain; }

    std::vector<VkRenderPass> GetRenderPasses()
    { return renderPasses; }

    VkFormat GetSwapchainImageFormat()
    { return swapchainImageFormat; }

    VkExtent2D GetSwapchainExtent()
    { return swapchainExtent; }

  private:
    void CleanupSwapChain();

    void createSwapChain(VkSwapchainKHR oldSwapChain);

    VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats);
    VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes);
    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities);

    std::unique_ptr<Device> &device;
    std::unique_ptr<QueueManager> &queueManager;
    std::unique_ptr<FrameManager> &frameManager;
    VkSwapchainKHR swapchain;
    std::vector<VkRenderPass> renderPasses;
    VkFormat swapchainImageFormat;
    VkExtent2D swapchainExtent;
  };

}
