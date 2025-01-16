#pragma once
#include "PipelineInfo.hpp"
#include "Vulkan/Device.hpp"
#include "crow_lib.hpp"
#include "VulkanTypes.hpp"

#include <functional>
#include <algorithm>
#include <unordered_map>

namespace vulkan
{
  class BufferManager
  {
  public:
    BufferManager(std::shared_ptr<Device> device, std::shared_ptr<QueueManager> queueManager);
    ~BufferManager();

    void CreateBuffer(BufferType bufferType, size_t dataSize);
    void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
    void DestroyBuffer(const int index);

  private:
    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

    std::shared_ptr<QueueManager> queueManager;
    std::shared_ptr<Device> device;

    std::vector<VulkanBuffer> buffers;
  };
}