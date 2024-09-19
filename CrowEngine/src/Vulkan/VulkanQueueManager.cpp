#include "VulkanQueueManager.hpp"
#include "../config/SettingsManager.hpp"
#include "../Logger.hpp"

namespace crowe
{
  void VulkanQueueManager::FindQueueFamilies(VkPhysicalDevice& physicDevice, VkSurfaceKHR& surface) {
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(physicDevice, &queueFamilyCount, nullptr);
    std::vector<VkQueueFamilyProperties> queueFamilyProperties(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(physicDevice, &queueFamilyCount, queueFamilyProperties.data());

    std::vector<QueueFamily> currentQueueFamilies;

    currentQueueFamilies.resize(queueFamilyCount);
    for (uint32_t i = 0; i < queueFamilyProperties.size(); i++)
    {
      currentQueueFamilies[i].index = i;
    }

    AssignQueuesToQueueFamilies(physicDevice, surface, VK_QUEUE_GRAPHICS_BIT, currentQueueFamilies, queueFamilyProperties, presentQueues, getQueueConfig().presentQueuesCount, true);
    AssignQueuesToQueueFamilies(physicDevice, surface, VK_QUEUE_GRAPHICS_BIT, currentQueueFamilies, queueFamilyProperties, graphicsQueues, getQueueConfig().graphicsQueuesCount, false);
    AssignQueuesToQueueFamilies(physicDevice, surface, VK_QUEUE_COMPUTE_BIT, currentQueueFamilies, queueFamilyProperties, computeQueues, getQueueConfig().computeQueuesCount, false);
    AssignQueuesToQueueFamilies(physicDevice, surface, VK_QUEUE_TRANSFER_BIT, currentQueueFamilies, queueFamilyProperties, transferQueues, getQueueConfig().transferQueuesCount, false);
    CleanupEmptyFamilies(currentQueueFamilies);

    queueFamilies = currentQueueFamilies; //Allocate to the heap without fragmentation
  }

  void VulkanQueueManager::AssignQueuesToQueueFamilies(
      VkPhysicalDevice& physicDevice,
      VkSurfaceKHR& surface,
      VkQueueFlagBits flagBit,
      std::vector<QueueFamily>& currentQueueFamilies,
      const std::vector<VkQueueFamilyProperties>& queueFamilyProperties,
      std::vector<QueueData>& queueType,
      int queueCount, bool isPresentQueue)
  {
    int bestQueueFamilyindex = 0;
    int bestQueueFamilySupportLevel = CheckFlagSupportNum(queueFamilyProperties[0].queueFlags);
    for (int i = 0; i < queueCount; i++)
    {
      for (int j = 0; j < queueFamilyProperties.size(); j++) {
        int supportedQueueCount = queueFamilyProperties[j].queueCount - currentQueueFamilies[j].queueCount;
        if (supportedQueueCount <= 0) continue; // Skip if no queues are available

        if (isPresentQueue && (queueFamilyProperties[j].queueFlags & VK_QUEUE_GRAPHICS_BIT)) {
          VkBool32 presentSupport = VK_FALSE;
          vkGetPhysicalDeviceSurfaceSupportKHR(physicDevice, j, surface, &presentSupport);
          if (presentSupport) {
            bestQueueFamilyindex = j;
            continue;
          }
        }
        else if (queueFamilyProperties[j].queueFlags & flagBit) {
          int flagSupportLevel = CheckFlagSupportNum(queueFamilyProperties[j].queueFlags);

          if (flagSupportLevel < bestQueueFamilySupportLevel) {
            bestQueueFamilySupportLevel = flagSupportLevel;
            bestQueueFamilyindex = j;
          }
        }
      }
      QueueData data = CreateQueueData(bestQueueFamilyindex, currentQueueFamilies[bestQueueFamilyindex].queueCount, queueFamilyProperties[bestQueueFamilyindex].queueCount);
      currentQueueFamilies[bestQueueFamilyindex].queueCount++;
      queueType.push_back(data);
      queueCount--;
    }
  }

  QueueData VulkanQueueManager::CreateQueueData(int familyIndex, int queueIndex, int maxQueueCount)
  {
    QueueData data;
    data.queueIndex = queueIndex;
    data.familyIndex = familyIndex;
    while (data.queueIndex > maxQueueCount)
    {
      data.queueIndex--;
    }
    return data;
  }

  void VulkanQueueManager::CleanupEmptyFamilies(std::vector<QueueFamily>& currentQueueFamilies)
  {
    for (int i = 0; i < currentQueueFamilies.size(); i++)
    {
      if (currentQueueFamilies[i].queueCount <= 0)
      {
        currentQueueFamilies.erase(currentQueueFamilies.begin() + i);
        i--;
        WARNING("Erased QueueFamily");
      }
    }
  };

  int VulkanQueueManager::CheckFlagSupportNum(VkQueueFlags flags)
  {
    return ((flags & VK_QUEUE_GRAPHICS_BIT) ? 1 : 0) +
           ((flags & VK_QUEUE_COMPUTE_BIT) ? 1 : 0) +
           ((flags & VK_QUEUE_TRANSFER_BIT) ? 1 : 0) +
           ((flags & VK_QUEUE_SPARSE_BINDING_BIT) ? 1 : 0);
  }

  void VulkanQueueManager::CreateQueues(VkDevice &device)
  {
    GetQueueHandles(device);
    BindQueueDataToQueues(presentQueues);
    BindQueueDataToQueues(graphicsQueues);
    BindQueueDataToQueues(computeQueues);
    BindQueueDataToQueues(transferQueues);
  }

  void VulkanQueueManager::GetQueueHandles(VkDevice& device)
  {
    for (int i = 0; i < queueFamilies.size(); ++i) {
      queueFamilies[i].queues.resize(queueFamilies[i].queueCount);
      for (int j = 0; j < queueFamilies[i].queues.size(); j++)
      {
        vkGetDeviceQueue(device, queueFamilies[i].index, j, &queueFamilies[i].queues[j]);
      }
    }
  }

  void VulkanQueueManager::BindQueueDataToQueues(std::vector<QueueData>& queueType)
  {
    for (int i = 0; i < queueFamilies[i].queues.size(); ++i) {
      queueType[i].pQueue = &queueFamilies[queueType[i].familyIndex].queues[queueType[i].queueIndex];
    }
  }

  VkDeviceQueueCreateInfo VulkanQueueManager::CreateQueueInfo(QueueFamily family)
  {
    std::vector<float> queuePriorities(family.queueCount, 1.0f);
    VkDeviceQueueCreateInfo queueCreateInfo{};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = family.index;
    queueCreateInfo.queueCount = family.queueCount;
    queueCreateInfo.pQueuePriorities = queuePriorities.data();
    return queueCreateInfo;
  }

  bool VulkanQueueManager::CreateCommandPools(VkDevice& device) {
    for (int i = 0; i < queueFamilies.size(); i++)
    {
      VkCommandPoolCreateInfo poolInfo{};
      poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
      poolInfo.queueFamilyIndex = queueFamilies[i].index;
      poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

      if (vkCreateCommandPool(device, &poolInfo, nullptr, &queueFamilies[i].commandPool) != VK_SUCCESS) {
        FATAL_ERROR("Failed to create command pool!");
        return false;
      }
    }
    return true;
  }

  void VulkanQueueManager::AllocateCommandBuffers(VkDevice& device) {
    for (int i = 0; i < queueFamilies.size(); i++)
    {
      queueFamilies[i].commandBuffers.resize(MAX_FRAMES_IN_FLIGHT + 3);
      VkCommandBufferAllocateInfo allocInfo{};
      allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
      allocInfo.commandPool = queueFamilies[i].commandPool;
      allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;  // VK_COMMAND_BUFFER_LEVEL_PRIMARY or VK_COMMAND_BUFFER_LEVEL_SECONDARY
      allocInfo.commandBufferCount = (uint32_t)queueFamilies[i].commandBuffers.size();  // Allocate 'count' number of command buffers

      if (vkAllocateCommandBuffers(device, &allocInfo, queueFamilies[i].commandBuffers.data()) != VK_SUCCESS) {
        throw std::runtime_error("Failed to allocate command buffers.");
      }
    }
  }
}