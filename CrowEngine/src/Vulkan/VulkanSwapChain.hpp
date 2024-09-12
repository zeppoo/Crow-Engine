#pragma once

#include "../crow_lib.hpp"
#include "VulkanDevice.hpp"
#include "VulkanQueueManager.hpp"

namespace crowe
{
class VulkanSwapChain {
public:

  VulkanSwapChain(VulkanDevice& device, VulkanQueueManager& queueManager);
  ~VulkanSwapChain();
  void recreateSwapChain();

  VkSwapchainKHR getSwapchain() const { return swapchain; }
  VkRenderPass getRenderPass() const { return renderPass; }
  const std::vector<VkImage>& getSwapchainImages() const { return swapchainImages; }
  VkFormat getSwapchainImageFormat() const { return swapchainImageFormat; }
  VkExtent2D getSwapchainExtent() const { return swapchainExtent; }

private:
  void createSwapChain();
  void createRenderPass();
  void createImageViews();
  void createFramebuffers();
  VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats);
  VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes);
  VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities);

  VulkanDevice& device;
  VulkanQueueManager& queueManager;
  VkSwapchainKHR swapchain;
  VkRenderPass renderPass;
  std::vector<VkImage> swapchainImages;
  std::vector<VkImageView> swapchainImageViews;
  std::vector<VkFramebuffer> swapchainFramebuffers;
  VkFormat swapchainImageFormat;
  VkExtent2D swapchainExtent;
};

}
