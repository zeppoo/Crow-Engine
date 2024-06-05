#include "VulkanBackend.hpp"
#include "Device.hpp"
#include "VulkanDebugger.hpp"
#include "../Core/Window.hpp"

#include <cstring>

namespace crowe {

VkInstance vkInstance;
VkDevice logicDevice;
VkPhysicalDevice physicDevice;
VkPhysicalDeviceProperties gpuProperties;
VkSurfaceKHR surface;
VkSwapchainKHR swapchain;
VkDebugUtilsMessengerEXT debugMessenger;
std::vector<const char*> validationLayers = { "VK_LAYER_KHRONOS_validation" };

#ifdef NDEBUG // NDEBUG = No Debug
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif


VkDevice getLogicDevice() {
  return logicDevice;
}

VkPhysicalDevice getPhysicDevice() {
  return physicDevice;
}

VkSurfaceKHR getSurface() {
  return surface;
}

VkSwapchainKHR getSwapchain() {
  return swapchain;
}


void VulkanStartup()
{
  InitVulkan();
  if (enableValidationLayers) { setupDebugMessenger(); }
  surface = CreateVulkanSurface(vkInstance);

}

void InitVulkan()
{
  if (enableValidationLayers && !checkValidationLayerSupport())
  {
    throw std::runtime_error("validation layers requested, but not available!");
  }

  VkApplicationInfo appInfo{};
  appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  appInfo.pApplicationName = "Hello Vulkan";
  appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.pEngineName = "No Engine";
  appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.apiVersion = VK_API_VERSION_1_2;

  VkInstanceCreateInfo createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  createInfo.pApplicationInfo = &appInfo;

  auto extensions = getRequiredExtensions();
  createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
  createInfo.ppEnabledExtensionNames = extensions.data();

  VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
  if (enableValidationLayers)
  {
    createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
    createInfo.ppEnabledLayerNames = validationLayers.data();
    populateDebugMessengerCreateInfo(debugCreateInfo);
    createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT *) &debugCreateInfo;
  } else
  {
    createInfo.enabledLayerCount = 0;
    createInfo.pNext = nullptr;
  }

  if (vkCreateInstance(&createInfo, nullptr, &vkInstance) != VK_SUCCESS)
  {
    vkDestroyInstance(vkInstance, nullptr);
    glfwDestroyWindow(GetWindow());
    glfwTerminate();
    throw std::runtime_error("Failed to create Vulkan instance");
  }
}

bool checkValidationLayerSupport()
{
  uint32_t layerCount;
  vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

  std::vector<VkLayerProperties> availableLayers(layerCount);
  vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

  for (const char *layerName: validationLayers)
  {
    bool layerFound = false;

    for (const auto &layerProperties: availableLayers)
    {
      if (strcmp(layerName, layerProperties.layerName) == 0)
      {
        layerFound = true;
        break;
      }
    }

    if (!layerFound)
    {
      return false;
    }
  }

  return true;
}

std::vector<const char *> getRequiredExtensions()
{
  uint32_t glfwExtensionCount = 0;
  const char **glfwExtensions;
  glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

  std::vector<const char *> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

  if (enableValidationLayers)
  {
    extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
  }

  return extensions;
}

void setupDebugMessenger()
{
  VkDebugUtilsMessengerCreateInfoEXT createInfo;
  populateDebugMessengerCreateInfo(createInfo);

  if (CreateDebugUtilsMessengerEXT(vkInstance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS)
  {
    throw std::runtime_error("failed to set up debug messenger!");
  }
}

}