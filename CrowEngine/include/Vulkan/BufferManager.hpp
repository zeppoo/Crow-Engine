#pragma once
#include "PipelineInfo.hpp"
#include "Vulkan/Device.hpp"
#include "Vulkan/CommandBuffers.hpp"
#include "crow_lib.hpp"
#include "glm/glm.hpp"

#include <functional>
#include <algorithm>
#include <unordered_map>

namespace vulkan
{
  class BufferManager
  {
  public:
    BufferManager(std::shared_ptr<Device> device, std::shared_ptr<CommandBufferManager> cmdBufferManager);
    ~BufferManager();

    int CreateNewBuffer(VkBufferUsageFlagBits usage, void* data, size_t size);
    void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
    void DestroyBuffer(const int index);

  private:
    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

    std::shared_ptr<CommandBufferManager> cmdBufferManager;
    std::shared_ptr<Device> device;

    std::vector<Buffer> buffers;
  };
}