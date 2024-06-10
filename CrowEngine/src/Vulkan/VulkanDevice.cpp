#include "VulkanDevice.hpp"
#include "VulkanBackend.hpp"
#include "VulkanUtilities.hpp"

#include <set>

namespace crowe
{
  VulkanDevice::VulkanDevice(VkInstance instance)
  {
    FindPhysicalDevice(instance);
  }

  VulkanDevice::~VulkanDevice()
  {

  }

  void VulkanDevice::FindPhysicalDevice(VkInstance instance)
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
    VkDevice logicDevice;
    // Find queue families
    QueueFamilyIndices indices = findQueueFamilies(getPhysicDevice(), getSurface());

    // Ensure queue families are complete before proceeding
    if (!indices.isComplete())
    {
      throw std::runtime_error("failed to find all necessary queue families!");
    }

    // Unique queue families
    std::set<uint32_t> uniqueQueueFamilies = {
        indices.GraphicsQueue.value(),
        indices.ComputeQueue.value(),
        indices.TransferQueue.value()
    };

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    float queuePriority = 1.0f;

    // Create queue create info structures
    for (uint32_t queueFamily: uniqueQueueFamilies)
    {
      VkDeviceQueueCreateInfo queueCreateInfo{};
      queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
      queueCreateInfo.queueFamilyIndex = queueFamily;
      queueCreateInfo.queueCount = 1;
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
    if (vkCreateDevice(getPhysicDevice(), &createInfo, nullptr, &logicDevice) != VK_SUCCESS)
    {
      throw std::runtime_error("failed to create logical device!");
    }

    // Retrieve the queue handles
    vkGetDeviceQueue(logicDevice, indices.GraphicsQueue.value(), 0, &GraphicsQueue);
    vkGetDeviceQueue(logicDevice, indices.ComputeQueue.value(), 0, &ComputeQueue);
    vkGetDeviceQueue(logicDevice, indices.TransferQueue.value(), 0, &TransferQueue);
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

    QueueFamilyIndices indices = findQueueFamilies(device, getSurface());
    if (indices.isComplete())
    {
      score += 500; // Has all necessary queue families
    }

    if (CheckDeviceSuitability(device)) {
      score += 500; // Supports necessary extensions and Swapchain
    }

    std::cout << score << "\n";

    return score;
  }
}
