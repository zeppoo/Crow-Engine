#pragma once

#include "../crow_lib.hpp"

namespace crowe
{
  class VulkanDevice {
  public:
    VulkanDevice(VkInstance instance);
    ~VulkanDevice();

    VkDevice getDevice() const { return device; }
    VkPhysicalDevice getPhysicDevice() const { return physicDevice; }
    VkQueue getGraphicsQueue() const { return GraphicsQueue; }
    VkQueue getComputeQueue() const { return ComputeQueue; }
    VkQueue getTransferQueue() const { return TransferQueue; }

  private:
    void FindPhysicalDevice(VkInstance instance);
    void CreateLogicalDevice();

    bool checkDeviceExtensionSupport(VkPhysicalDevice physicDevice);
    bool CheckDeviceSuitability(VkPhysicalDevice device);
    int RateDevice(VkPhysicalDevice device);

    VkDevice device;
    VkPhysicalDevice physicDevice;
    VkQueue GraphicsQueue;
    VkQueue ComputeQueue;
    VkQueue TransferQueue;
  };
} // namespace crowe
