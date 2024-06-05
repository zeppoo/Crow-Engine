#ifndef SWAPCHAIN_HPP
#define SWAPCHAIN_HPP

#include "../crow_lib.hpp"

namespace crowe
{
void createSwapChain();
void createRenderPass();
VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats);
VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes);
VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities);
}

#endif //SWAPCHAIN_HPP
