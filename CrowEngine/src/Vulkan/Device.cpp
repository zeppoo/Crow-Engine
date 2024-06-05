#include "Device.hpp"
#include "VulkanBackend.hpp"

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

  for (const auto &device: devices)
  {
    if (CheckDeviceSuitability(device))
    {
      physicDevice = device;
      break;
    }
  }

  if (physicDevice == VK_NULL_HANDLE)
  {
    throw std::runtime_error("failed to find a suitable GPU!");
  } else {
    return physicDevice;
  }
}

void CreateLogicalDevice() {}

void CheckDeviceSuitability(VkPhysicalDevice device)
{
  QueueFamilyIndices indices = findQueueFamilies(device, GetSurface());

  bool extensionsSupported = checkDeviceExtensionSupport(physicDevice);

  bool swapChainAdequate = false;
  if (extensionsSupported)
  {
    SwapChainSupportDetails swapChainSupport = querySwapChainSupport(physicDevice, surface);
    swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
  }

  return indices.isComplete() && extensionsSupported && swapChainAdequate;
}

}
