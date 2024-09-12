#include "VulkanQueueManager.hpp"

namespace crowe
{

  VulkanQueueManager::VulkanQueueManager() {}

  VulkanQueueManager::~VulkanQueueManager()
  {
    /*for (int i = 0; i < queueFamilies.size(); i++)
    {
      vkDestroyCommandPool(device, queueFamilies[i].commandPool, nullptr);
    }*/
  }

  void VulkanQueueManager::FindQueueFamilies(VkPhysicalDevice& physicDevice, VkSurfaceKHR& surface) {
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(physicDevice, &queueFamilyCount, nullptr);
    std::vector<VkQueueFamilyProperties> queueFamilyProperties(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(physicDevice, &queueFamilyCount, queueFamilyProperties.data());

    /*std::cout << "Queue Family Properties:" << std::endl;
    std::cout << "----------------------------" << std::endl;

    for (uint32_t i = 0; i < queueFamilyCount; ++i) {
      std::cout << "Queue Family " << i << ":" << std::endl;

      // Queue count and capabilities (graphics, compute, transfer, sparse binding)
      std::cout << "  Queue Count: " << queueFamilyProperties[i].queueCount << std::endl;
      std::cout << "  Queue Flags:" << std::endl;

      if (queueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
        std::cout << "    - Graphics" << std::endl;
      }
      if (queueFamilyProperties[i].queueFlags & VK_QUEUE_COMPUTE_BIT) {
        std::cout << "    - Compute" << std::endl;
      }
      if (queueFamilyProperties[i].queueFlags & VK_QUEUE_TRANSFER_BIT) {
        std::cout << "    - Transfer" << std::endl;
      }
      if (queueFamilyProperties[i].queueFlags & VK_QUEUE_SPARSE_BINDING_BIT) {
        std::cout << "    - Sparse Binding" << std::endl;
      }

      // Timestamp valid bits (number of bits for timestamps in queries)
      std::cout << "  Timestamp Valid Bits: " << queueFamilyProperties[i].timestampValidBits << std::endl;

      // Minimum granularity for image transfers (valid for transfer queues)
      std::cout << "  Min Image Transfer Granularity: "
                << queueFamilyProperties[i].minImageTransferGranularity.width << " x "
                << queueFamilyProperties[i].minImageTransferGranularity.height << " x "
                << queueFamilyProperties[i].minImageTransferGranularity.depth << std::endl;

      std::cout << std::endl;
    }*/

    queueFamilies.resize(queueFamilyCount);
    AssignFamilyQueues(physicDevice, surface, VK_QUEUE_GRAPHICS_BIT, queueFamilyProperties, graphicsFamilies);
    AssignFamilyQueues(physicDevice, surface, VK_QUEUE_COMPUTE_BIT, queueFamilyProperties, computeFamilies);
    AssignFamilyQueues(physicDevice, surface, VK_QUEUE_TRANSFER_BIT, queueFamilyProperties, transferFamilies);
  }

  void VulkanQueueManager::AssignFamilyQueues(VkPhysicalDevice& physicDevice, VkSurfaceKHR& surface, VkQueueFlagBits flagBit, const std::vector<VkQueueFamilyProperties>& queueFamilyProperties, std::vector<uint32_t>& queueType)
  {
    for (uint32_t i = 0; i < queueFamilyProperties.size(); i++) {
      queueFamilies[i].queues.resize(queueFamilyProperties[i].queueCount);
      VkQueueFlags flags = queueFamilyProperties[i].queueFlags;
      int flagsSupported = CheckFlagSupportNum(flags);

      if (flags & VK_QUEUE_GRAPHICS_BIT) {
        VkBool32 presentSupport = VK_FALSE;
        vkGetPhysicalDeviceSurfaceSupportKHR(physicDevice, i, surface, &presentSupport);
        if (presentSupport)
        {
          queueFamilies[i].queueFamilyIndex = i;
          queueType.push_back(i);
          continue;
        }
      }
      if ((flags & flagBit) == flagBit && flagsSupported == 1) {
        queueFamilies[i].flags.push_back(flagBit);
        queueFamilies[i].queueFamilyIndex = i;
        queueType.push_back(i);
      }
      else if ((flags & flagBit) == flagBit && flagsSupported <= 3) {
        queueFamilies[i].flags.push_back(flagBit);
        queueFamilies[i].queueFamilyIndex = i;
        queueType.push_back(i);
      }
      else if ((flags & flagBit) == flagBit && flagsSupported >= 4) {
        if (queueType.size() == 0)
        {
          queueFamilies[i].flags.push_back(flagBit);
          queueFamilies[i].queueFamilyIndex = i;
          queueType.push_back(i);
        }
      }
    }
  }

  int VulkanQueueManager::CheckFlagSupportNum(VkQueueFlags flags)
  {
    int num = 0;
    if (flags & VK_QUEUE_GRAPHICS_BIT) {
      num++;
    }
    if (flags & VK_QUEUE_COMPUTE_BIT) {
      num++;
    }
    if (flags & VK_QUEUE_TRANSFER_BIT) {
      num++;
    }
    if (flags & VK_QUEUE_SPARSE_BINDING_BIT) {
      num++;
    }

    return num;
  }

  void VulkanQueueManager::CreateQueues()
  {

  }

  /*void VulkanQueueManager::createCommandPool(uint32_t i) {
    CommandPoolData commandPoolData;
    commandPoolData.queueIndex = i;
    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.queueFamilyIndex = i;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

    if (vkCreateCommandPool(device, &poolInfo, nullptr, &commandPoolData.commandPool) != VK_SUCCESS) {
      throw std::runtime_error("Failed to create command pool!");
    }
    allocateCommandBuffers(&commandPoolData);
    commandPools.push_back(commandPoolData);
  }

  void VulkanQueueManager::allocateCommandBuffers(CommandPoolData* commandPoolData) {
    commandPoolData->commandBuffers.resize(MAX_FRAMES_IN_FLIGHT + 3);

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = commandPoolData->commandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;  // VK_COMMAND_BUFFER_LEVEL_PRIMARY or VK_COMMAND_BUFFER_LEVEL_SECONDARY
    allocInfo.commandBufferCount = (uint32_t)commandPoolData->commandBuffers.size();  // Allocate 'count' number of command buffers

    if (vkAllocateCommandBuffers(device, &allocInfo, commandPoolData->commandBuffers.data()) != VK_SUCCESS) {
      throw std::runtime_error("Failed to allocate command buffers.");
    }
  }*/
}