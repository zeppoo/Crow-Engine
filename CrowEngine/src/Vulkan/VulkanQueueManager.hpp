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
    VkQueue queue;
    int queueindex;
    int queueFamilyIndex;
  };

  struct QueueFamily
  {
    int queueFamilyIndex;
    int queueCount = 0;
    VkCommandPool commandPool;
    std::vector<VkCommandBuffer> commandBuffers;
  };

  class VulkanQueueManager {
  public:
    void FindQueueFamilies(VkPhysicalDevice& physicDevice, VkSurfaceKHR& surface);
    void CreateQueues(VkDevice& device);
    bool CreateCommandPools(VkDevice& device);
    void AllocateCommandBuffers(VkDevice& device);

    std::vector<QueueFamily> GetQueueFamilies() {return queueFamilies;}

  private:
    // Helper function to create a command pool
    void AssignQueues(VkPhysicalDevice& physicDevice, VkSurfaceKHR& surface, VkQueueFlagBits flagBit, const std::vector<VkQueueFamilyProperties>& queueFamilyProperties, std::vector<QueueData>& queueType, int indicies, bool isPresentQueue);
    void CleanupEmptyFamilies();
    int CheckFlagSupportNum(VkQueueFlags flags);
    void GetQueueHandles(VkDevice& device, std::vector<QueueData>& queueType);

    std::vector<QueueFamily> queueFamilies;

    std::vector<QueueData> presentQueues;
    std::vector<QueueData> graphicsQueues;
    std::vector<QueueData> computeQueues;
    std::vector<QueueData> transferQueues;
  };
}

