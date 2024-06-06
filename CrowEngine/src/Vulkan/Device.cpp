#include "Device.hpp"
#include "VulkanBackend.hpp"
#include "Queues.hpp"
#include "SwapChain.hpp"

#include <set>

namespace crowe
{
  VkPhysicalDevice FindPhysicalDevice(VkInstance instance)
  {
    uint32_t deviceCount = 0;
    VkPhysicalDevice physicDevice;

    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
    if (deviceCount == 0)
    {
      throw std::runtime_error("failed to find GPUs with Vulkan support!");
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

    int bestScore = 0;
    for (const auto &device: devices)
    {
      if (RateDevice(device) > bestScore)
      {
        physicDevice = device;
      }
    }

    if (physicDevice == VK_NULL_HANDLE)
    {
      throw std::runtime_error("failed to find a suitable GPU!");
    } else {
      VkPhysicalDeviceProperties properties;
      vkGetPhysicalDeviceProperties(physicDevice, &properties);
      std::cout << "\n\nDevice Name: " << properties.deviceName << ":\n";
      return physicDevice;
    }
  }

  void CreateLogicalDevice() {}

  bool checkDeviceExtensionSupport(VkPhysicalDevice physicDevice)
  {
    std::vector<const char*> deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(physicDevice, nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(physicDevice, nullptr, &extensionCount, availableExtensions.data());

    std::set<std::string> requiredExtensions((deviceExtensions).begin(), (deviceExtensions).end());

    for (const auto &extension: availableExtensions)
    {
      requiredExtensions.erase(extension.extensionName);
    }

    return requiredExtensions.empty();
  }

  bool CheckDeviceSuitability(VkPhysicalDevice device)
  {
    bool extensionsSupported = checkDeviceExtensionSupport(device);

    bool swapChainAdequate = false;
    if (extensionsSupported)
    {
      SwapChainSupportDetails swapChainSupport = querySwapChainSupport(device, getSurface());
      swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
    }

    return extensionsSupported && swapChainAdequate;
  }

  int RateDevice(VkPhysicalDevice device)
  {
    int score = 0;

    VkPhysicalDeviceProperties deviceProperties;
    VkPhysicalDeviceFeatures deviceFeatures;
    vkGetPhysicalDeviceProperties(device, &deviceProperties);
    vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

    if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
      score += 1000; // Discrete GPUs have the highest performance
    }

    // Maximum dimension of 2D images
    score += deviceProperties.limits.maxImageDimension2D;

    if (deviceFeatures.geometryShader) {
      score += 100; // Support for geometry shaders
    }

    QueueFamilyIndices indices = findQueueFamilies(device, getSurface());
    if (indices.isComplete())
    {
      score += 500; // Has all necessary queue families
    }

    if (CheckDeviceSuitability(device)) {
      score += 500; // Supports necessary extensions and Swapchain
    }

    return score;
  }
}
