#pragma once

#include "../crow_lib.hpp"

namespace crowe
{
  struct VulkanQueue{
    VkQueue queue;
    uint32_t queueIndex;
    VkCommandPool commandPool;
    std::vector<VkCommandBuffer> commandBuffers;
  };

  class VulkanDevice {
  public:
    VulkanDevice(VkInstance instance);
    ~VulkanDevice();

    VkDevice getDevice() const { return device; }
    VkPhysicalDevice getPhysicDevice() const { return physicDevice; }
    VulkanQueue getGraphicsQueue() const { return GraphicsQueue; }
    VulkanQueue getComputeQueue() const { return ComputeQueue; }
    VulkanQueue getTransferQueue() const { return TransferQueue; }

  private:
    void FindPhysicalDevice(VkInstance instance);
    void CreateLogicalDevice();

    void createCommanPool(VulkanQueue* vulkanQueue);
    void createCommandBuffers(VulkanQueue* vulkanQueue);
    bool checkDeviceExtensionSupport(VkPhysicalDevice physicDevice);
    bool CheckDeviceSuitability(VkPhysicalDevice device);
    int RateDevice(VkPhysicalDevice device);

    VkDevice device;
    VkPhysicalDevice physicDevice;
    VulkanQueue GraphicsQueue;
    VulkanQueue ComputeQueue;
    VulkanQueue TransferQueue;
  };
} // namespace crowe
