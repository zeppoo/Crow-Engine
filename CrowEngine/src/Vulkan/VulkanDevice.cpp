#include "VulkanDevice.hpp"
#include "VulkanBackend.hpp"
#include "VulkanUtilities.hpp"

#include <set>
#include <cstring>

namespace crowe
{
  VulkanDevice::VulkanDevice(std::unique_ptr<VulkanQueueManager>& queueManager) : queueManager{queueManager}
  {
    std::cout << "Setting up Vulkan Device" << std::endl;
    InitVulkan();
    surface = CreateVulkanSurface(vkInstance);
    SetupLogicalDevice();
  }

  VulkanDevice::~VulkanDevice()
  {
    vkDestroyDevice(device, nullptr);

    if (enableValidationLayers) {
      DestroyDebugUtilsMessengerEXT(vkInstance, debugMessenger, nullptr);
    }

    vkDestroySurfaceKHR(vkInstance, surface, nullptr);
    vkDestroyInstance(vkInstance, nullptr);
  }

  void VulkanDevice::InitVulkan()
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

  void VulkanDevice::SetupLogicalDevice()
  {
    FindPhysicalDevice();
    CreateLogicalDevice();
  }

  void VulkanDevice::FindPhysicalDevice()
  {
    uint32_t deviceCount = 0;

    vkEnumeratePhysicalDevices(vkInstance, &deviceCount, nullptr);
    if (deviceCount == 0)
    {
      throw std::runtime_error("failed to find GPUs with Vulkan support!");
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(vkInstance, &deviceCount, devices.data());

    int bestScore = 0;
    for (const auto &device: devices)
    {
      int currentScore = RateDevice(device);
      if (currentScore > bestScore)
      {
        bestScore = currentScore;
        physicDevice = device;
      }
    }

    if (physicDevice == VK_NULL_HANDLE)
    {
      throw std::runtime_error("failed to find a suitable GPU!");
    } else {
      VkPhysicalDeviceProperties properties;
      vkGetPhysicalDeviceProperties(physicDevice, &properties);
      std::cout << "\n\nChosen Device: " << properties.deviceName << ":\n";
    }
  }

  void VulkanDevice::CreateLogicalDevice()
  {
    queueManager->FindQueueFamilies(physicDevice, surface);

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    float queuePriority = 1.0f;

    // Create queue create info structures
    for (QueueFamily family : queueManager->GetQueueFamilies())
    {
      VkDeviceQueueCreateInfo queueCreateInfo{};
      queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
      queueCreateInfo.queueFamilyIndex = family.queueFamilyIndex;
      queueCreateInfo.queueCount = family.queues.size();
      queueCreateInfo.pQueuePriorities = &queuePriority;
      queueCreateInfos.push_back(queueCreateInfo);
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
    if (getEnableValidationLayers())
    {
      createInfo.enabledLayerCount = static_cast<uint32_t>(getValidationLayers().size());
      createInfo.ppEnabledLayerNames = getValidationLayers().data();
    } else
    {
      createInfo.enabledLayerCount = 0;
    }

    // Create the logical device
    if (vkCreateDevice(getPhysicDevice(), &createInfo, nullptr, &device) != VK_SUCCESS)
    {
      throw std::runtime_error("failed to create logical device!");
    }

    std::cout << "Vulkan Device is Setup" << std::endl;
  }

  bool VulkanDevice::checkDeviceExtensionSupport(VkPhysicalDevice physicDevice)
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

  bool VulkanDevice::CheckDeviceSuitability(VkPhysicalDevice device)
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

    /*std::cout << "Physical Device Properties:" << std::endl;
    std::cout << "----------------------------" << std::endl;
    std::cout << "Device Name: " << deviceProperties.deviceName << std::endl;
    std::cout << "API Version: "
              << VK_VERSION_MAJOR(deviceProperties.apiVersion) << "."
              << VK_VERSION_MINOR(deviceProperties.apiVersion) << "."
              << VK_VERSION_PATCH(deviceProperties.apiVersion) << std::endl;
    std::cout << "Driver Version: "
              << VK_VERSION_MAJOR(deviceProperties.driverVersion) << "."
              << VK_VERSION_MINOR(deviceProperties.driverVersion) << "."
              << VK_VERSION_PATCH(deviceProperties.driverVersion) << std::endl;
    std::cout << "Vendor ID: " << deviceProperties.vendorID << std::endl;
    std::cout << "Device ID: " << deviceProperties.deviceID << std::endl;
    std::cout << "Device Type: " << deviceProperties.deviceType << std::endl;
    std::cout << "Max Image Dimension 2D: " << deviceProperties.limits.maxImageDimension2D << std::endl;
    std::cout << std::endl;

    // Print more detailed device limits
    std::cout << "Device Limits:" << std::endl;
    std::cout << "----------------------------" << std::endl;
    std::cout << "Max Image Dimension 3D: " << deviceProperties.limits.maxImageDimension3D << std::endl;
    std::cout << "Max Image Dimension Cube: " << deviceProperties.limits.maxImageDimensionCube << std::endl;
    std::cout << "Max Uniform Buffer Range: " << deviceProperties.limits.maxUniformBufferRange << std::endl;
    std::cout << "Max Vertex Input Attributes: " << deviceProperties.limits.maxVertexInputAttributes << std::endl;
    std::cout << "Max Viewports: " << deviceProperties.limits.maxViewports << std::endl;
    std::cout << "Max Compute Work Group Count: "
              << deviceProperties.limits.maxComputeWorkGroupCount[0] << ", "
              << deviceProperties.limits.maxComputeWorkGroupCount[1] << ", "
              << deviceProperties.limits.maxComputeWorkGroupCount[2] << std::endl;
    std::cout << std::endl;

    // Print Physical Device Features
    std::cout << "Physical Device Features:" << std::endl;
    std::cout << "----------------------------" << std::endl;
    std::cout << "Geometry Shader: " << (deviceFeatures.geometryShader ? "Supported" : "Not Supported") << std::endl;
    std::cout << "Tessellation Shader: " << (deviceFeatures.tessellationShader ? "Supported" : "Not Supported") << std::endl;
    std::cout << "Wide Lines: " << (deviceFeatures.wideLines ? "Supported" : "Not Supported") << std::endl;
    std::cout << "Large Points: " << (deviceFeatures.largePoints ? "Supported" : "Not Supported") << std::endl;
    std::cout << "Multi Viewport: " << (deviceFeatures.multiViewport ? "Supported" : "Not Supported") << std::endl;
    std::cout << "Sampler Anisotropy: " << (deviceFeatures.samplerAnisotropy ? "Supported" : "Not Supported") << std::endl;
    std::cout << "Shader Clip Distance: " << (deviceFeatures.shaderClipDistance ? "Supported" : "Not Supported") << std::endl;
    std::cout << "Shader Cull Distance: " << (deviceFeatures.shaderCullDistance ? "Supported" : "Not Supported") << std::endl;
    std::cout << "Depth Bounds: " << (deviceFeatures.depthBounds ? "Supported" : "Not Supported") << std::endl;
    std::cout << "Fill Mode Non-Solid: " << (deviceFeatures.fillModeNonSolid ? "Supported" : "Not Supported") << std::endl;
    std::cout << "Depth Clamp: " << (deviceFeatures.depthClamp ? "Supported" : "Not Supported") << std::endl;
    std::cout << "Multi Draw Indirect: " << (deviceFeatures.multiDrawIndirect ? "Supported" : "Not Supported") << std::endl;
    std::cout << "Draw Indirect First Instance: " << (deviceFeatures.drawIndirectFirstInstance ? "Supported" : "Not Supported") << std::endl;
    std::cout << std::endl;*/

    return score;
  }
  
  bool VulkanDevice::checkValidationLayerSupport()
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

  bool VulkanDevice::checkExtensionSupport(const std::vector<const char*>& requiredExtensions) {
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
