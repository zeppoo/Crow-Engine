#include "Vulkan/VulkanDevice.hpp"
#include "Vulkan/VulkanUtilities.hpp"
#include "Vulkan/VulkanDebugger.hpp"
#include "Config/SettingsManager.hpp"
#include "Logger.hpp"

#include <set>
#include <cstring>

namespace crowe
{
  VulkanDevice::VulkanDevice(std::unique_ptr<Window> &window, std::unique_ptr<VulkanQueueManager> &queueManager)
      : queueManager{queueManager}
  {
    InitVulkan();
    INFO("Succesfully Created Vulkan Instance");
    surface = window->CreateVulkanSurface(vkInstance);
    SetupDevice();
  }

  bool VulkanDevice::InitVulkan()
  {
    if (!checkValidationLayerSupport()) {
      FATAL_ERROR("Validation layers requested, but not available!");
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
    const char **glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    std::vector<const char *> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
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
    if (getEnableValidationLayers()) {
      createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
      createInfo.ppEnabledLayerNames = validationLayers.data();
      populateDebugMessengerCreateInfo(debugCreateInfo);
      createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT *) &debugCreateInfo;
    } else {
      createInfo.enabledLayerCount = 0;
      createInfo.pNext = nullptr;
    }

    if (vkCreateInstance(&createInfo, nullptr, &vkInstance) != VK_SUCCESS) {
      FATAL_ERROR("Failed to create Vulkan instance!");
    }

    if (getEnableValidationLayers() &&
        CreateDebugUtilsMessengerEXT(vkInstance, &debugCreateInfo, nullptr, &debugMessenger) != VK_SUCCESS) {
      FATAL_ERROR("failed to set up debug messenger!");
      return false;
    }

    return true;
  }

  void VulkanDevice::SetupDevice()
  {
    if (FindPhysicalDevice()) {
      INFO("Found Physical Device!");
    }
    if (CreateLogicalDevice()) {
      INFO("Created Logical Device!");
    }
    if (queueManager->CreateCommandPools(device)) {
      INFO("Created Command Pools!");
    }
  }

  bool VulkanDevice::FindPhysicalDevice()
  {
    uint32_t deviceCount = 0;

    vkEnumeratePhysicalDevices(vkInstance, &deviceCount, nullptr);
    if (deviceCount == 0) {
      throw std::runtime_error("failed to find GPUs with Vulkan support!");
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(vkInstance, &deviceCount, devices.data());

    int bestScore = 0;
    for (const auto &device: devices) {
      int currentScore = RateDevice(device);
      if (currentScore > bestScore) {
        bestScore = currentScore;
        physicDevice = device;
      }
    }

    if (physicDevice == VK_NULL_HANDLE) {
      FATAL_ERROR("failed to find a suitable GPU!");
      return false;
    } else {
      VkPhysicalDeviceProperties properties;
      vkGetPhysicalDeviceProperties(physicDevice, &properties);
      return true;
    }
  }

  bool VulkanDevice::CreateLogicalDevice()
  {
    queueManager->FindQueueFamilies(physicDevice, surface);

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;

    // Create queue create info structures
    for (int i = 0; i < queueManager->GetQueueFamilies().size(); i++) {
      VkDeviceQueueCreateInfo info = queueManager->CreateQueueInfo(queueManager->GetQueueFamilies()[i]);
      queueCreateInfos.push_back(info);
    }

    // Device features
    VkPhysicalDeviceFeatures deviceFeatures{};

    // Device create info
    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
    createInfo.pQueueCreateInfos = queueCreateInfos.data();
    createInfo.pEnabledFeatures = &deviceFeatures;
    createInfo.enabledExtensionCount = static_cast<uint32_t>((getDeviceExtensions()).size());
    createInfo.ppEnabledExtensionNames = (getDeviceExtensions()).data();

    // Validation layers
    if (getEnableValidationLayers()) {
      createInfo.enabledLayerCount = static_cast<uint32_t>(getValidationLayers().size());
      createInfo.ppEnabledLayerNames = getValidationLayers().data();
    } else {
      createInfo.enabledLayerCount = 0;
    }

    // Create the logical device
    if (vkCreateDevice(getPhysicDevice(), &createInfo, nullptr, &device) != VK_SUCCESS) {
      FATAL_ERROR("Failed to Create Logical Device");
      return false;
    }

    queueManager->CreateQueues(device);
    return true;
  }

  bool VulkanDevice::checkDeviceExtensionSupport(VkPhysicalDevice physicDevice)
  {
    std::vector<const char *> deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(physicDevice, nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(physicDevice, nullptr, &extensionCount, availableExtensions.data());

    std::set<std::string> requiredExtensions((deviceExtensions).begin(), (deviceExtensions).end());

    for (const auto &extension: availableExtensions) {
      requiredExtensions.erase(extension.extensionName);
    }

    return requiredExtensions.empty();
  }

  bool VulkanDevice::CheckDeviceSuitability(VkPhysicalDevice device)
  {
    bool extensionsSupported = checkDeviceExtensionSupport(device);

    bool swapChainAdequate = false;
    if (extensionsSupported) {
      SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(device, getSurface());
      swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
    }

    return extensionsSupported && swapChainAdequate;
  }

  int VulkanDevice::RateDevice(VkPhysicalDevice device)
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

    if (CheckDeviceSuitability(device)) {
      score += 500; // Supports necessary extensions and Swapchain
    }

    return score;
  }

  bool VulkanDevice::checkValidationLayerSupport()
  {
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    for (const char *layerName: validationLayers) {
      bool layerFound = false;

      for (const auto &layerProperties: availableLayers) {
        if (strcmp(layerName, layerProperties.layerName) == 0) {
          layerFound = true;
          break;
        }
      }

      if (!layerFound) {
        return false;
      }
    }

    return true;
  }

  bool VulkanDevice::checkExtensionSupport(const std::vector<const char *> &requiredExtensions)
  {
    uint32_t extensionCount;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, availableExtensions.data());

    for (const char *requiredExtension: requiredExtensions) {
      bool extensionFound = false;

      for (const auto &extensionProperties: availableExtensions) {
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