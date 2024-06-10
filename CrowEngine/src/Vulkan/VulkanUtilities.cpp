#include "VulkanUtilities.hpp"

namespace crowe
{
  SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface){
    SwapChainSupportDetails details;

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

    if (formatCount != 0)
    {
      details.formats.resize(formatCount);
      vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
    }

    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);

    if (presentModeCount != 0)
    {
      details.presentModes.resize(presentModeCount);
      vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());
    }
    return details;
  }

  QueueFamilyIndices findQueueFamilies(VkPhysicalDevice physicDevice, VkSurfaceKHR surface) {
    QueueFamilyIndices queueFamilyIndices;

    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(physicDevice, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(physicDevice, &queueFamilyCount, queueFamilies.data());

    VkPhysicalDeviceProperties properties;
    vkGetPhysicalDeviceProperties(physicDevice, &properties);
    std::cout << "\n\nDevice Name: " << properties.deviceName << ":\n";
    std::cout << "Device Type: " << properties.deviceType << ":\n";
    std::cout << "API Version: " << properties.apiVersion << ":\n";
    std::cout << "Driver Version " << properties.driverVersion << ":\n";
    std::cout << "Max Memory Allocation Amount: " << properties.limits.maxMemoryAllocationCount << ":\n";

      for (uint32_t i = 0; i < queueFamilyCount; i++) {
        const VkQueueFamilyProperties& queueFamily = queueFamilies[i];

        if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
          std::cout << "Queue Family " << i << ":\n";
          std::cout << "  Has Independent Graphics Queue\n";
          queueFamilyIndices.GraphicsQueue = i;
          VkBool32 presentSupport = false;
          vkGetPhysicalDeviceSurfaceSupportKHR(physicDevice, i, surface, &presentSupport);
          if (presentSupport)
          {
            std::cout << "    Supports Presentation\n";
          }
        }
        if (queueFamilies[i].queueFlags & VK_QUEUE_COMPUTE_BIT &&
            !(queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)) {
          std::cout << "Queue Family " << i << ":\n";
          std::cout << "  Has Independent Compute Queue\n";
          queueFamilyIndices.ComputeQueue = i;
        }
        if (queueFamilies[i].queueFlags & VK_QUEUE_TRANSFER_BIT &&
            !(queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) &&
            !(queueFamilies[i].queueFlags & VK_QUEUE_COMPUTE_BIT)) {
          std::cout << "Queue Family " << i << ":\n";
          std::cout << "  Has Independent Transfer Queue\n";
          queueFamilyIndices.TransferQueue = i;
        }
      }

    return queueFamilyIndices;
  }
}