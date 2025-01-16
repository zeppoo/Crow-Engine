#pragma once

#include "crow_lib.hpp"

namespace vulkan
{
  struct Info {
    int key;
    std::string name;
    std::vector<double> infos;
  };

  struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
  };

  SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface);

  uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
}
