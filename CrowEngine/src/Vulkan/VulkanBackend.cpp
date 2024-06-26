#include "VulkanBackend.hpp"
#include "VulkanDevice.hpp"
#include "VulkanSwapChain.hpp"
#include "VulkanGraphicsPipeline.hpp"
#include "VulkanDebugger.hpp"
#include "../Core/Window.hpp"

#include <cstring>
#include <memory>

namespace crowe {

VkInstance vkInstance = VK_NULL_HANDLE;
VkSurfaceKHR surface;
VkDebugUtilsMessengerEXT debugMessenger;
std::unique_ptr<VulkanDevice> device;
std::unique_ptr<VulkanSwapChain> swapchain;
//std::unique_ptr<VulkanGraphicsPipeline> pipeline;

#ifdef NDEBUG // NDEBUG = No Debug
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

std::vector<const char*> validationLayers = { "VK_LAYER_KHRONOS_validation" };
std::vector<const char*> deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

VkInstance getVkInstance() { return vkInstance; }
VkSurfaceKHR getSurface() { return surface; }
const bool getEnableValidationLayers() { return enableValidationLayers; }
const std::vector<const char*>& getValidationLayers() { return validationLayers; }
const std::vector<const char*>& getDeviceExtensions() { return deviceExtensions; }

void VulkanStartup()
{
  InitVulkan();
  surface = CreateVulkanSurface(vkInstance);
  device = std::make_unique<VulkanDevice>(vkInstance);
  //swapchain = std::make_unique<VulkanSwapChain>(*device, surface);
  //pipeline = std::make_unique<VulkanGraphicsPipeline>();
}

void CleanupVulkan()
{

}

void InitVulkan()
{
  if (!checkValidationLayerSupport()) {
    throw std::runtime_error("Validation layers requested, but not available!");
  }

  VkApplicationInfo appInfo{};
  appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  appInfo.pApplicationName = "Hello Vulkan";
  appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.pEngineName = "No Engine";
  appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.apiVersion = VK_API_VERSION_1_0;

  VkInstanceCreateInfo createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  createInfo.pApplicationInfo = &appInfo;

  uint32_t glfwExtensionCount = 0;
  const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
  std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
  extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
  if (!checkExtensionSupport(extensions)) {
    throw std::runtime_error("Required extensions not supported!");
  }
  createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
  createInfo.ppEnabledExtensionNames = extensions.data();

  if (checkValidationLayerSupport()) {
    createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
    createInfo.ppEnabledLayerNames = validationLayers.data();
  } else {
    createInfo.enabledLayerCount = 0;
  }

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

  if (vkCreateInstance(&createInfo, nullptr, &vkInstance) != VK_SUCCESS) {
    throw std::runtime_error("Failed to create Vulkan instance!");
  }

  if (enableValidationLayers && CreateDebugUtilsMessengerEXT(vkInstance, &debugCreateInfo, nullptr, &debugMessenger) != VK_SUCCESS)
  {
    throw std::runtime_error("failed to set up debug messenger!");
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

bool checkExtensionSupport(const std::vector<const char*>& requiredExtensions) {
  uint32_t extensionCount;
  vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

  std::vector<VkExtensionProperties> availableExtensions(extensionCount);
  vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, availableExtensions.data());

  for (const char* requiredExtension : requiredExtensions) {
    bool extensionFound = false;

    for (const auto& extensionProperties : availableExtensions) {
      if (strcmp(requiredExtension, extensionProperties.extensionName) == 0) {
        extensionFound = true;
        break;
      }
    }

    if (!extensionFound) {
      return false;
    }
  }

  return true;
}
}