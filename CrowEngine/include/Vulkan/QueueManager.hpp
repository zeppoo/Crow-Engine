#pragma once

#include <unordered_set>
#include <optional>
#include "crow_lib.hpp"

namespace vulkan
{
enum QueueType { PRESENT, GRAPHICS, COMPUTE, TRANSFER };

struct QueueData {
  VkFence queueFence;
  std::vector<VkCommandBuffer> recordedBuffers;
  VkQueue *pQueue;
  int queueIndex;
  int familyIndex;
};

struct QueueFamily {
  int index;
  int queueCount = 0;
  std::vector<VkQueue> queues;
  std::vector<float> queuePriorities;
  VkCommandPool commandPool = VK_NULL_HANDLE;
  std::vector<VkCommandBuffer> commandBuffers = {VK_NULL_HANDLE};
  int currentBufferIndex = 0;
};

class QueueManager {
public:
  void FindQueueFamilies(VkPhysicalDevice &physicDevice, VkSurfaceKHR &surface);

  void AllocateQueues(VkDevice &device);

  bool CreateCommandPools(VkDevice &device);

  bool AllocateCommandBuffers(VkDevice &device);

  std::vector<VkDeviceQueueCreateInfo> CreateQueueInfos();

  VkCommandBuffer* GetCommandBuffer(QueueType bufferType, uint32_t queueIndex);

  void QueueCommandBuffer(VkCommandBuffer *commandBuffer, QueueType queueType, uint32_t queueIndex);

  void SubmitQueuedBuffers(VkDevice device, QueueType queueType, uint32_t queueIndex);

  std::vector<QueueFamily> GetQueueFamilies() { return queueFamilies; }

  std::vector<QueueData> GetPresentQueues() { return presentQueues; }

  std::vector<QueueData> GetGraphicsQueues() { return graphicsQueues; }

  std::vector<QueueData> GetComputeQueues() { return computeQueues; }

  std::vector<QueueData> GetTransferQueues() { return transferQueues; }

private:
  void AssignQueuesToQueueFamilies(VkPhysicalDevice &physicDevice, VkSurfaceKHR &surface, VkQueueFlagBits flagBit,
                                   std::vector<QueueFamily> &currentQueueFamilies,
                                   const std::vector<VkQueueFamilyProperties> &queueFamiliesProperties, std::vector<QueueData> &queueType,
                                   int indicies, bool isPresentQueue);

  QueueData CreateQueueData(QueueFamily &family, int maxQueueCount);

  void CleanupEmptyFamilies(std::vector<QueueFamily> &currentQueueFamilies);

  int CheckFlagSupportNum(VkQueueFlags flags);

  void GetQueueHandles(VkDevice &device);

  void BindQueueDataToQueues(std::vector<QueueData> &queueList);

  void CreateQueueFences(VkDevice &device, std::vector<QueueData> &queueList);

  std::vector<QueueFamily> queueFamilies;
  std::vector<QueueData> presentQueues;
  std::vector<QueueData> graphicsQueues;
  std::vector<QueueData> computeQueues;
  std::vector<QueueData> transferQueues;
};
} // namespace vulkan
