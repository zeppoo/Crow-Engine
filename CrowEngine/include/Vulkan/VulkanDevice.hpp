#pragma once

#include "../General/crow_lib.hpp"
#include "../Core/Window.hpp"

#include "VulkanQueueManager.hpp"
#include <memory>

namespace crowe
{
  class VulkanDevice {
  public:
    VulkanDevice(std::unique_ptr<Window>& window, std::unique_ptr<VulkanQueueManager>& queueManager);

    // Getters
    VkInstance getVkInstance() { return vkInstance; }
    VkSurfaceKHR getSurface() { return surface; }
    VkDevice getDevice() const { return device; }
    VkPhysicalDevice getPhysicDevice() const { return physicDevice; }
    VkDebugUtilsMessengerEXT getDebugMessenger() const {return debugMessenger;}
    const std::vector<const char*>& getValidationLayers() { return validationLayers; }
    const std::vector<const char*>& getDeviceExtensions() { return deviceExtensions; }

  private:
    bool InitVulkan();
    void SetupDevice();
    bool FindPhysicalDevice();
    bool CreateLogicalDevice();
    bool checkDeviceExtensionSupport(VkPhysicalDevice physicDevice);
    bool CheckDeviceSuitability(VkPhysicalDevice device);
    int RateDevice(VkPhysicalDevice device);
    bool checkValidationLayerSupport();
    bool checkExtensionSupport(const std::vector<const char*>& requiredExtensions);

    //References
    std::unique_ptr<VulkanQueueManager>& queueManager;
    // Members
    VkInstance vkInstance;
    VkSurfaceKHR surface;
    VkDevice device;
    VkPhysicalDevice physicDevice;
    VkDebugUtilsMessengerEXT debugMessenger;
    std::vector<const char*> validationLayers = { "VK_LAYER_KHRONOS_validation" };
    std::vector<const char*> deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
  };
}