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

    queueFamilies.resize(queueFamilyCount);
    for (uint32_t i = 0; i < queueFamilyProperties.size(); i++)
    {
      queueFamilies[i].queueFamilyIndex = i;
    }

    AssignQueues(physicDevice, surface, VK_QUEUE_GRAPHICS_BIT, queueFamilyProperties, presentQueues, getQueueConfig().presentQueuesCount, true);
    AssignQueues(physicDevice, surface, VK_QUEUE_GRAPHICS_BIT, queueFamilyProperties, graphicsQueues, getQueueConfig().graphicsQueuesCount, false);
    AssignQueues(physicDevice, surface, VK_QUEUE_COMPUTE_BIT, queueFamilyProperties, computeQueues, getQueueConfig().computeQueuesCount, false);
    AssignQueues(physicDevice, surface, VK_QUEUE_TRANSFER_BIT, queueFamilyProperties, transferQueues, getQueueConfig().transferQueuesCount, false);

    CleanupEmptyFamilies();
  }

  void VulkanQueueManager::AssignQueues(VkPhysicalDevice& physicDevice, VkSurfaceKHR& surface, VkQueueFlagBits flagBit, const std::vector<VkQueueFamilyProperties>& queueFamilyProperties, std::vector<QueueData>& queueType, int queueCount, bool isPresentQueue)
  {
    uint32_t bestQueueFamilyindex;
    int bestQueueFamilySupportLevel = CheckFlagSupportNum(queueFamilyProperties[0].queueFlags);
    //To Do: zorg dat hij checkt of er een specifieke family, en niet alleen kijkt of hij de flagbit support
    for (int i = 0; i < queueCount; i++)
    {
      for (uint32_t j = 0; j < queueFamilyProperties.size(); j++) {
        int supportedQueueCount = queueFamilyProperties[j].queueCount - queueFamilies[j].queueCount;
        if (supportedQueueCount <= 0) continue; // Skip if no queues are available

        if (isPresentQueue && (queueFamilyProperties[j].queueFlags & VK_QUEUE_GRAPHICS_BIT)) {
          VkBool32 presentSupport = VK_FALSE;
          vkGetPhysicalDeviceSurfaceSupportKHR(physicDevice, j, surface, &presentSupport);
          if (presentSupport) {
            bestQueueFamilyindex = j;
            continue;
          }
        }

        if (queueFamilyProperties[j].queueFlags & flagBit) {
          int flagSupportLevel = CheckFlagSupportNum(queueFamilyProperties[j].queueFlags);

          if (flagSupportLevel < bestQueueFamilySupportLevel) {
            bestQueueFamilySupportLevel = flagSupportLevel;
            bestQueueFamilyindex = j;
          }
        }
      }
      QueueData data = {VK_NULL_HANDLE, queueFamilies[bestQueueFamilyindex].queueCount - 1, bestQueueFamilyindex};
      queueFamilies[bestQueueFamilyindex].queueCount++;
      queueType.push_back(data);
      queueCount--;
    }
  }

  void VulkanQueueManager::CleanupEmptyFamilies()
  {
    for (int i = 0; i < queueFamilies.size(); i++)
    {
      if (queueFamilies[i].queueCount <= 0)
      {
        queueFamilies.erase(queueFamilies.begin() + i);
        i--;
        WARNING("Erased QueueFamily: " + std::to_string(i));
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
    GetQueueHandles(device, presentQueues);
    GetQueueHandles(device, graphicsQueues);
    GetQueueHandles(device, computeQueues);
    GetQueueHandles(device, transferQueues);
  }

  void VulkanQueueManager::GetQueueHandles(VkDevice& device, std::vector<QueueData>& queueType)
  {
    for (int i = 0; i < queueType.size(); i++)
    {
        vkGetDeviceQueue(device, queueType[i].queueFamilyIndex, queueType[i].queueindex, &queueType[i].queue);
    }
  }

  bool VulkanQueueManager::CreateCommandPools(VkDevice& device) {
    for (int i = 0; i < queueFamilies.size(); i++)
    {
      VkCommandPoolCreateInfo poolInfo{};
      poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
      poolInfo.queueFamilyIndex = queueFamilies[i].queueFamilyIndex;
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