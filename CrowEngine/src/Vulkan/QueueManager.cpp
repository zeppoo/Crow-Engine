#include "Vulkan/QueueManager.hpp"
#include "Vulkan/FrameManager.hpp"
#include "Config/SettingsManager.hpp"
#include "Logger.hpp"

namespace vulkan
{
  void QueueManager::FindQueueFamilies(VkPhysicalDevice &physicDevice, VkSurfaceKHR &surface)
  {
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(physicDevice, &queueFamilyCount, nullptr);
    std::vector<VkQueueFamilyProperties> queueFamilyProperties(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(physicDevice, &queueFamilyCount, queueFamilyProperties.data());

    std::vector<QueueFamily> currentQueueFamilies;

    currentQueueFamilies.resize(queueFamilyCount);
    for (uint32_t i = 0; i < queueFamilyProperties.size(); i++) {
      currentQueueFamilies[i].index = i;
    }

    AssignQueuesToQueueFamilies(physicDevice, surface, VK_QUEUE_GRAPHICS_BIT, currentQueueFamilies,
                                queueFamilyProperties, presentQueues, settings::getQueueConfig().presentQueuesCount, true);
    AssignQueuesToQueueFamilies(physicDevice, surface, VK_QUEUE_GRAPHICS_BIT, currentQueueFamilies,
                                queueFamilyProperties, graphicsQueues, settings::getQueueConfig().graphicsQueuesCount, false);
    AssignQueuesToQueueFamilies(physicDevice, surface, VK_QUEUE_COMPUTE_BIT, currentQueueFamilies,
                                queueFamilyProperties, computeQueues, settings::getQueueConfig().computeQueuesCount, false);
    AssignQueuesToQueueFamilies(physicDevice, surface, VK_QUEUE_TRANSFER_BIT, currentQueueFamilies,
                                queueFamilyProperties, transferQueues, settings::getQueueConfig().transferQueuesCount, false);
    CleanupEmptyFamilies(currentQueueFamilies);

    queueFamilies = currentQueueFamilies; // Allocate to prevent memory fragmentation
  }

  void QueueManager::AssignQueuesToQueueFamilies(
      VkPhysicalDevice &physicDevice,
      VkSurfaceKHR &surface,
      VkQueueFlagBits flagBit,
      std::vector<QueueFamily> &currentQueueFamilies,
      const std::vector<VkQueueFamilyProperties> &queueFamiliesProperties,
      std::vector<QueueData> &queueType,
      int queueCount,
      bool isPresentQueue)
  {
    int bestQueueFamilyindex = 0;
    // I look for how many flags the first family supports.
    // this makes sure that a queue gets assigned to the family that is most dedicated to the flag we're looking for
    int bestQueueFamilySupportLevel = CheckFlagSupportNum(queueFamiliesProperties[0].queueFlags);
    for (int i = 0; i < queueCount; i++) { // Iterate for the amount of queues we're looking to allocate
      for (int j = 0; j < queueFamiliesProperties.size(); j++) { // Iterate over every family we found
        int supportedQueueCount = queueFamiliesProperties[j].queueCount - currentQueueFamilies[j].queueCount; // Check if current family has enough queues left to assign
        if (supportedQueueCount <= 0) continue; // Skip if no queues are available

        if (isPresentQueue && (queueFamiliesProperties[j].queueFlags & VK_QUEUE_GRAPHICS_BIT)) {
          VkBool32 presentSupport = VK_FALSE;
          vkGetPhysicalDeviceSurfaceSupportKHR(physicDevice, j, surface, &presentSupport);
          if (presentSupport) {
            bestQueueFamilyindex = j;
            continue;
          }
        } else if (queueFamiliesProperties[j].queueFlags & flagBit) {
          int flagSupportLevel = CheckFlagSupportNum(queueFamiliesProperties[j].queueFlags);
          // Check if this family is more dedicated than the previous best family
          if (flagSupportLevel < bestQueueFamilySupportLevel) {
            bestQueueFamilySupportLevel = flagSupportLevel;
            bestQueueFamilyindex = j;
          }
        }
      }
      QueueData data = CreateQueueData(currentQueueFamilies[bestQueueFamilyindex],
                                       queueFamiliesProperties[bestQueueFamilyindex].queueCount);
      currentQueueFamilies[bestQueueFamilyindex].queueCount++;
      currentQueueFamilies[bestQueueFamilyindex].queuePriorities.push_back(1.0f);
      queueType.push_back(data);
      queueCount--;
    }
  }

  int QueueManager::CheckFlagSupportNum(VkQueueFlags flags)
  {
    return ((flags & VK_QUEUE_GRAPHICS_BIT) ? 1 : 0) +
           ((flags & VK_QUEUE_COMPUTE_BIT) ? 1 : 0) +
           ((flags & VK_QUEUE_TRANSFER_BIT) ? 1 : 0) +
           ((flags & VK_QUEUE_SPARSE_BINDING_BIT) ? 1 : 0);
  }

  QueueData QueueManager::CreateQueueData(QueueFamily &family, int maxQueueCount)
  {
    QueueData data;
    data.queueIndex = family.queueCount;
    data.familyIndex = family.index;
    while (data.queueIndex >= maxQueueCount) {
      family.queueCount--;
      data.queueIndex--;
    }
    return data;
  }

  void QueueManager::CleanupEmptyFamilies(std::vector<QueueFamily> &currentQueueFamilies)
  {
    for (int i = 0; i < currentQueueFamilies.size(); i++) {
      if (currentQueueFamilies[i].queueCount <= 0) {
        currentQueueFamilies.erase(currentQueueFamilies.begin() + i);
        i--;
        logger::Warning("Erased QueueFamily");
      }
    }
  };

  void QueueManager::AllocateQueues(VkDevice &device)
  {
    GetQueueHandles(device);
    BindQueueDataToQueues(presentQueues);
    BindQueueDataToQueues(graphicsQueues);
    BindQueueDataToQueues(computeQueues);
    BindQueueDataToQueues(transferQueues);
    CreateQueueFences(device, presentQueues);
    CreateQueueFences(device, graphicsQueues);
    CreateQueueFences(device, computeQueues);
    CreateQueueFences(device, transferQueues);
  }

  void QueueManager::GetQueueHandles(VkDevice &device)
  {
    for (int i = 0; i < queueFamilies.size(); ++i) {
      queueFamilies[i].queues.resize(queueFamilies[i].queueCount);
      for (int j = 0; j < queueFamilies[i].queues.size(); j++) {
        vkGetDeviceQueue(device, queueFamilies[i].index, j, &queueFamilies[i].queues[j]);
      }
    }
  }

  void QueueManager::BindQueueDataToQueues(std::vector<QueueData> &queueList)
  {
    for (int i = 0; i < queueFamilies[i].queues.size(); ++i) {
      queueList[i].pQueue = &queueFamilies[queueList[i].familyIndex].queues[queueList[i].queueIndex];
    }
  }

  void QueueManager::CreateQueueFences(VkDevice &device, std::vector<QueueData> &queueList)
  {
    for (int i = 0; i < queueFamilies[i].queues.size(); ++i) {
      VkFenceCreateInfo fenceInfo{};
      fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
      fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

      if (vkCreateFence(device, &fenceInfo, nullptr, &queueList[i].queueFence) != VK_SUCCESS) {
        logger::FatalError("Failed to create queue fences");
      }
    }
  }

  std::vector<VkDeviceQueueCreateInfo> QueueManager::CreateQueueInfos()
  {
    std::vector<VkDeviceQueueCreateInfo> queue_create_infos;

    for (auto & queueFamilie : queueFamilies) {
      VkDeviceQueueCreateInfo queueCreateInfo{};
      queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
      queueCreateInfo.queueFamilyIndex = queueFamilie.index;
      queueCreateInfo.queueCount = queueFamilie.queueCount;
      queueCreateInfo.pQueuePriorities = queueFamilie.queuePriorities.data();
      queue_create_infos.push_back(queueCreateInfo);
    }
    return queue_create_infos;
  }

  bool QueueManager::CreateCommandPools(VkDevice &device)
  {
    for (int i = 0; i < queueFamilies.size(); i++) {
      VkCommandPoolCreateInfo poolInfo{};
      poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
      poolInfo.queueFamilyIndex = queueFamilies[i].index;
      poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

      if (vkCreateCommandPool(device, &poolInfo, nullptr, &queueFamilies[i].commandPool) != VK_SUCCESS) {
        logger::FatalError("Failed to create command pool!");
        return false;
      }
    }
    return true;
  }

  bool QueueManager::AllocateCommandBuffers(VkDevice &device)
  {
    for (int i = 0; i < queueFamilies.size(); i++) {
      queueFamilies[i].commandBuffers.resize(FRAMES_IN_FLIGHT + 3);
      VkCommandBufferAllocateInfo allocInfo{};
      allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
      allocInfo.commandPool = queueFamilies[i].commandPool;
      allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;  // VK_COMMAND_BUFFER_LEVEL_PRIMARY or VK_COMMAND_BUFFER_LEVEL_SECONDARY
      allocInfo.commandBufferCount = (uint32_t) queueFamilies[i].commandBuffers.size();  // Allocate 'count' number of command buffers

      if (vkAllocateCommandBuffers(device, &allocInfo, queueFamilies[i].commandBuffers.data()) != VK_SUCCESS) {
        logger::FatalError("Failed to allocate command buffers.");
        return false;
      }
    }

    return true;
  }

  VkCommandBuffer* QueueManager::GetCommandBuffer(QueueType bufferType)
  {
    switch(bufferType)
    {
      case PRESENT:
        return &queueFamilies[presentQueues[0].familyIndex].commandBuffers[0];
      case GRAPHICS:
        return &queueFamilies[graphicsQueues[0].familyIndex].commandBuffers[0];
      case COMPUTE:
        return &queueFamilies[computeQueues[0].familyIndex].commandBuffers[0];
      case TRANSFER:
        return &queueFamilies[transferQueues[0].familyIndex].commandBuffers[0];
      default:
        logger::Warning("Can't find right command buffer type");
        return nullptr;
    }
  }

  void QueueManager::SubmitCommandBuffer(VkCommandBuffer* commandBuffer, QueueType queueType)
  {
    switch(queueType)
    {
      case PRESENT:
        presentQueues[0].recordedBuffers.push_back(*commandBuffer);
      case GRAPHICS:
        graphicsQueues[0].recordedBuffers.push_back(*commandBuffer);
      case COMPUTE:
        computeQueues[0].recordedBuffers.push_back(*commandBuffer);
      case TRANSFER:
        transferQueues[0].recordedBuffers.push_back(*commandBuffer);
      default:
        logger::Warning("Couldn't submit commandbuffer");
    }
  }
}