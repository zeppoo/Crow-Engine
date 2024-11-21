#pragma once

#include "crow_lib.hpp"
#include "Core/Window.hpp"
#include "QueueManager.hpp"

namespace vulkan
{
  class Device {
  public:
    Device(std::unique_ptr<core::Window> &window, std::unique_ptr<QueueManager> &queueManager);

    VkInstance getVkInstance()
    { return vkInstance; }

    VkSurfaceKHR getSurface()
    { return surface; }

    VkDevice getDevice() const
    { return device; }

    VkPhysicalDevice getPhysicDevice() const
    { return physicDevice; }

    VkDebugUtilsMessengerEXT getDebugMessenger() const
    { return debugMessenger; }

    const std::vector<const char *> &getValidationLayers()
    { return validationLayers; }

    const std::vector<const char *> &getDeviceExtensions()
    { return deviceExtensions; }

  private:
    bool InitVulkan();

    void SetupDevice();

    bool FindPhysicalDevice();

    bool CreateLogicalDevice();

    bool checkDeviceExtensionSupport(VkPhysicalDevice physicDevice);

    bool CheckDeviceSuitability(VkPhysicalDevice device);

    int RateDevice(VkPhysicalDevice device);

    bool checkValidationLayerSupport();

    bool checkExtensionSupport(const std::vector<const char *> &requiredExtensions);

    //References
    std::unique_ptr<QueueManager> &queueManager;
    // Members
    VkInstance vkInstance;
    VkSurfaceKHR surface;
    VkDevice device;
    VkPhysicalDevice physicDevice;
    VkDebugUtilsMessengerEXT debugMessenger;
    std::vector<const char *> validationLayers = {"VK_LAYER_KHRONOS_validation"};
    std::vector<const char *> deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
  };
}