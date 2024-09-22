#pragma once

#include "../crow_lib.hpp"
#include "VulkanDevice.hpp"
#include "VulkanQueueManager.hpp"

namespace crowe
{
class VulkanSwapChain {
public:

  VulkanSwapChain(std::unique_ptr<VulkanDevice>& device, std::unique_ptr<VulkanQueueManager>& queueManager);
  ~VulkanSwapChain();
  void SetupSwapChain();
  void RecreateSwapChain();

  VkSwapchainKHR GetSwapchain() const { return swapchain; }
  VkRenderPass GetRenderPass() const { return renderPass; }
  const std::vector<VkImage> GetSwapchainImages() const { return swapchainImages; }
  std::vector<VkImageView> GetSwapchainImageViews() const {return swapchainImageViews;}
  VkFormat GetSwapchainImageFormat() const { return swapchainImageFormat; }
  VkExtent2D GetSwapchainExtent() const { return swapchainExtent; }

private:
  void createSwapChain(VkSwapchainKHR oldSwapChain);
  void createRenderPass();
  void createImageViews();
  void createFramebuffers();
  VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats);
  VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes);
  VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities);

  std::unique_ptr<VulkanDevice>& device;
  std::unique_ptr<VulkanQueueManager>& queueManager;
  VkSwapchainKHR swapchain;
  VkRenderPass renderPass;
  std::vector<VkImage> swapchainImages;
  std::vector<VkImageView> swapchainImageViews;
  std::vector<VkFramebuffer> swapchainFramebuffers;
  VkFormat swapchainImageFormat;
  VkExtent2D swapchainExtent;
};

}
