#include "Queues.hpp"

namespace crowe {

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

      std::cout << "Queue Family " << i << ":\n";
      std::cout << "  Queue Count: " << queueFamily.queueCount << "\n";
      std::cout << "  Queue Flags: " << queueFamily.queueFlags << "\n";

      if (queueFamily.queueFlags & queueFamilyIndices.Bits[1]) {
        std::cout << "    Supports Graphics\n";
        queueFamilyIndices.queue1 = i;
      }
      if (queueFamily.queueFlags & queueFamilyIndices.Bits[2]) {
        std::cout << "    Supports Compute\n";
        queueFamilyIndices.queue2 = i;
      }
      VkBool32 presentSupport = false;
      vkGetPhysicalDeviceSurfaceSupportKHR(physicDevice, i, surface, &presentSupport);
      if (presentSupport)
      {
        std::cout << "    Supports Presentation\n";
      }

      std::cout << "  Timestamp Valid Bits: " << queueFamily.timestampValidBits << "\n";
      std::cout << "  Min Image Transfer Granularity: ("
                << queueFamily.minImageTransferGranularity.width << ", "
                << queueFamily.minImageTransferGranularity.height << ", "
                << queueFamily.minImageTransferGranularity.depth << ")\n";
    }

    return queueFamilyIndices;
  }
}