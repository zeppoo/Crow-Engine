#pragma once

#include "../crow_lib.hpp"
#include <optional>

namespace crowe
{
  enum QueueType
  {
    GRAPHICS,
    COMPUTE,
    TRANSFER
  };

  struct QueueData{
    VkQueue queue;
    uint32_t queueindex;
  };

  struct QueueFamily
  {
    uint32_t queueFamilyIndex;
    std::vector<VkQueueFlagBits> flags;
    std::vector<QueueData> queues;
    VkCommandPool commandPool;
    std::vector<VkCommandBuffer> commandBuffers;
  };

  class VulkanQueueManager {
  public:
    VulkanQueueManager();
    ~VulkanQueueManager();
    void FindQueueFamilies(VkPhysicalDevice& physicDevice, VkSurfaceKHR& surface);
    void CreateQueues(VkDevice& device);
    void CreateCommandPools(VkDevice& device);
    void AllocateCommandBuffers(VkDevice& device);

    std::vector<QueueFamily> GetQueueFamilies() {return queueFamilies;}

  private:
    // Helper function to create a command pool
    void AssignFamilyQueues(VkPhysicalDevice& physicDevice, VkSurfaceKHR& surface, VkQueueFlagBits flagBit, const std::vector<VkQueueFamilyProperties>& queueFamilyProperties, std::vector<uint32_t>& queueType);
    int CheckFlagSupportNum(VkQueueFlags flags);


    std::vector<QueueFamily> queueFamilies;

    std::vector<uint32_t> graphicsFamilies;
    std::vector<uint32_t> computeFamilies;
    std::vector<uint32_t> transferFamilies;
  };
}

