#pragma once

#include "crow_lib.hpp"
#include "Device.hpp"
#include "QueueManager.hpp"
#include "Config/RenderPassSettings.hpp"

namespace vulkan
{
  struct SwapchainImage {
    VkImage image;
    VkImageView view;
    VkFramebuffer framebuffer;
  };

  class SwapChain {
  public:
    SwapChain(std::unique_ptr<Device> &device, std::unique_ptr<QueueManager> &queueManager);

    ~SwapChain();

    void SetupSwapChain();

    void RecreateSwapChain();

    void CreateRenderPass(RenderPassConfig& RenderPassConfig);
    void BeginRenderPass(VkCommandBuffer* pCommandBuffer, uint32_t renderpassIndex, uint32_t currentImage);
    void SetViewPort(VkCommandBuffer* pCommandBuffer);
    void SetScissor(VkCommandBuffer* pCommandBuffer);
    void EndRenderPass(VkCommandBuffer* pCommandBuffer);

    VkSwapchainKHR GetSwapchain()
    { return swapchain; }

    std::vector<SwapchainImage> GetSwapchainImages()
    { return swapchainImages; }

    std::vector<VkRenderPass> GetRenderPasses()
    { return renderPasses; }

    VkFormat GetSwapchainImageFormat()
    { return swapchainImageFormat; }

    VkExtent2D GetSwapchainExtent()
    { return swapchainExtent; }

  private:
    void CleanupSwapChain();

    void createSwapChain(VkSwapchainKHR oldSwapChain);
    void CreateSwapchainImages();
    void CreateImageView(uint32_t index);
    void CreateFrameBuffer(uint32_t index);
    VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats);
    VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes);
    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities);


    std::unique_ptr<Device> &device;
    std::unique_ptr<QueueManager> &queueManager;
    VkSwapchainKHR swapchain;
    std::vector<VkRenderPass> renderPasses;
    std::vector<SwapchainImage> swapchainImages;
    VkFormat swapchainImageFormat;
    VkExtent2D swapchainExtent;
  };

}
