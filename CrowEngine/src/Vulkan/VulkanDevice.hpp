#pragma once

#include "../crow_lib.hpp"
#include "VulkanQueueManager.hpp"
#include <memory>

namespace crowe
{
  class VulkanDevice {
  public:
    VulkanDevice(std::unique_ptr<VulkanQueueManager>& queueManager);
    ~VulkanDevice();

    void SetupLogicalDevice();
    void FindPhysicalDevice();
    void CreateLogicalDevice();

    // Getters
    VkInstance getVkInstance() { return vkInstance; }
    VkSurfaceKHR getSurface() { return surface; }
    VkDevice getDevice() const { return device; }
    VkPhysicalDevice getPhysicDevice() const { return physicDevice; }
    const std::vector<const char*>& getValidationLayers() { return validationLayers; }
    const std::vector<const char*>& getDeviceExtensions() { return deviceExtensions; }

  private:
    bool checkDeviceExtensionSupport(VkPhysicalDevice physicDevice);
    bool CheckDeviceSuitability(VkPhysicalDevice device);
    int RateDevice(VkPhysicalDevice device);
    void InitVulkan();
    bool checkValidationLayerSupport();
    bool checkExtensionSupport(const std::vector<const char*>& requiredExtensions);

    std::unique_ptr<VulkanQueueManager>& queueManager;
    VkInstance vkInstance;
    VkSurfaceKHR surface;
    VkDevice device;
    VkPhysicalDevice physicDevice;
    VkDebugUtilsMessengerEXT debugMessenger;
    std::vector<const char*> validationLayers = { "VK_LAYER_KHRONOS_validation" };
    std::vector<const char*> deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
  };
}
