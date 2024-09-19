#pragma once

#include "../crow_lib.hpp"

#include <optional>

namespace crowe
{
  enum QueueType
  {
    PRESENT,
    GRAPHICS,
    COMPUTE,
    TRANSFER
  };

  struct QueueData{
    VkQueue* pQueue;
    int queueIndex;
    int familyIndex;
  };

  struct QueueFamily
  {
    int index;
    int queueCount = 0;
    std::vector<VkQueue> queues;
    VkCommandPool commandPool;
    std::vector<VkCommandBuffer> commandBuffers;
  };

  class VulkanQueueManager {
  public:
    void FindQueueFamilies(VkPhysicalDevice& physicDevice, VkSurfaceKHR& surface);
    void CreateQueues(VkDevice& device);
    bool CreateCommandPools(VkDevice& device);
    void AllocateCommandBuffers(VkDevice& device);
    VkDeviceQueueCreateInfo CreateQueueInfo(QueueFamily family);

    std::vector<QueueFamily> GetQueueFamilies() {return queueFamilies;}

  private:
    // Helper function to create a command pool
    void AssignQueuesToQueueFamilies(
        VkPhysicalDevice& physicDevice,
        VkSurfaceKHR& surface,
        VkQueueFlagBits flagBit,
        std::vector<QueueFamily>& currentQueueFamilies,
        const std::vector<VkQueueFamilyProperties>& queueFamilyProperties,
        std::vector<QueueData>& queueType,
        int indicies, bool isPresentQueue);

    QueueData CreateQueueData(QueueFamily& family, int maxQueueCount);
    void CleanupEmptyFamilies(std::vector<QueueFamily>& currentQueueFamilies);
    int CheckFlagSupportNum(VkQueueFlags flags);
    void GetQueueHandles(VkDevice& device);
    void BindQueueDataToQueues(std::vector<QueueData>& queueType);


    std::vector<QueueFamily> queueFamilies;

    std::vector<QueueData> presentQueues;
    std::vector<QueueData> graphicsQueues;
    std::vector<QueueData> computeQueues;
    std::vector<QueueData> transferQueues;
  };
}