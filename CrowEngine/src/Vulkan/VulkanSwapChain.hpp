#pragma once

#include "../crow_lib.hpp"
#include "VulkanDevice.hpp"

namespace crowe
{
class VulkanSwapChain {
public:

  VulkanSwapChain(VulkanDevice& device, VkSurfaceKHR surface);
  ~VulkanSwapChain();
  void recreate();

  VkSwapchainKHR getSwapchain() const { return swapchain; }
  const std::vector<VkImage>& getSwapchainImages() const { return swapchainImages; }
  VkFormat getSwapchainImageFormat() const { return swapchainImageFormat; }
  VkExtent2D getSwapchainExtent() const { return swapchainExtent; }

private:
  void createSwapChain();
  void createRenderPass();
  VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats);
  VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes);
  VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities);

  VkSwapchainKHR swapchain;
  std::vector<VkImage> swapchainImages;
  VkFormat swapchainImageFormat;
  VkExtent2D swapchainExtent;

  friend class VulkanFriend;
};

}
