#include "VulkanDevice.hpp"
#include "VulkanBackend.hpp"
#include "VulkanUtilities.hpp"

#include <set>

namespace crowe
{
  VulkanDevice::VulkanDevice(VkInstance instance)
  {
    std::cout << "Setting up Vulkan Device" << std::endl;
    FindPhysicalDevice(instance);
    CreateLogicalDevice();
  }

  VulkanDevice::~VulkanDevice()
  {

  }

  void VulkanDevice::FindPhysicalDevice(VkInstance instance)
  {
    uint32_t deviceCount = 0;

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
    // Find queue families
    QueueFamilyIndices indices = findQueueFamilies(physicDevice, getSurface());

    GraphicsQueue.queueIndex = indices.graphicsFamily.value();
    ComputeQueue.queueIndex = indices.computeFamily.value();
    TransferQueue.queueIndex = indices.transferFamily.value();

    // Unique queue families
    std::set<uint32_t> uniqueQueueFamilies = {
        indices.graphicsFamily.value(),
        indices.computeFamily.value(),
        indices.transferFamily.value()
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
    if (vkCreateDevice(getPhysicDevice(), &createInfo, nullptr, &device) != VK_SUCCESS)
    {
      throw std::runtime_error("failed to create logical device!");
    }

    // Retrieve the queue handles
    if (indices.isComplete())
    {
      vkGetDeviceQueue(device, indices.graphicsFamily.value(), 0, &GraphicsQueue.queue);
      createCommanPool(&GraphicsQueue);
      createCommandBuffers(&GraphicsQueue);
      vkGetDeviceQueue(device, indices.computeFamily.value(), 0, &ComputeQueue.queue);
      createCommanPool(&ComputeQueue);
      createCommandBuffers(&ComputeQueue);
      vkGetDeviceQueue(device, indices.transferFamily.value(), 0, &TransferQueue.queue);
      createCommanPool(&TransferQueue);
      createCommandBuffers(&TransferQueue);
    }
    else
    {
      throw std::runtime_error("GPU doesn't support necessary queue families");
    }

    std::cout << "Vulkan Device is Setup" << std::endl;
  }

  void VulkanDevice::createCommanPool(VulkanQueue* vulkanQueue)
  {
    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.queueFamilyIndex = vulkanQueue->queueIndex;
    poolInfo.flags = 0;

    if (vkCreateCommandPool(device, &poolInfo, nullptr, &vulkanQueue->commandPool) != VK_SUCCESS)
    {
      throw std::runtime_error("failed to create command pool!");
    }
  }

  void VulkanDevice::createCommandBuffers(crowe::VulkanQueue *vulkanQueue)
  {
    vulkanQueue->commandBuffers.resize(MAX_FRAMES_IN_FLIGHT);

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = vulkanQueue->commandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = (uint32_t) vulkanQueue->commandBuffers.size();

    if (vkAllocateCommandBuffers(device, &allocInfo, vulkanQueue->commandBuffers.data()) != VK_SUCCESS)
    {
      throw std::runtime_error("failed to allocate command buffers!");
    }
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

    return score;
  }
}
