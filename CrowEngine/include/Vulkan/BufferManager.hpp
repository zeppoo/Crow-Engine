#pragma once
#include "PipelineInfo.hpp"
#include "Vulkan/Device.hpp"
#include "crow_lib.hpp"
#include "glm/glm.hpp"

#include <functional>
#include <algorithm>
#include <unordered_map>

namespace vulkan
{
  class Buffer {
  public:
    struct CreateInfo {
      VkDeviceSize size;
      VkBufferUsageFlags usage;
      VkMemoryPropertyFlags memoryProperties;
      bool createStaging = false;
      // You could add more parameters like:
      // - Custom alignment requirements
      // - Sharing mode/queue families
      // - Debug name
    };

    virtual ~Buffer() = default;

    virtual void create(const CreateInfo& info) = 0;
    virtual void destroy() = 0;
    virtual void update(const void* data, VkDeviceSize size, VkDeviceSize offset = 0) = 0;

    VkBuffer getHandle() const { return buffer; }
    VkDeviceSize getSize() const { return size; }

  protected:
    VkBuffer buffer = VK_NULL_HANDLE;
    VkDeviceMemory memory = VK_NULL_HANDLE;
    VkDeviceSize size = 0;
  };

  class BufferManager
  {
  public:
    BufferManager(std::shared_ptr<Device> device, std::shared_ptr<QueueManager> queueManager);
    ~BufferManager();

    void AddCommandBuffer(const std::string name, QueueType type);
    void DestroyCommandBuffer(const std::string name);

    void CreateNewBuffer(const std::string name, Buffer& buffer);
    void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
    void DestroyBuffer(const std::string name);

  private:
    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

    std::shared_ptr<QueueManager> queueManager;
    std::shared_ptr<Device> device;

    std::unordered_map<std::string, Buffer> buffers;
  };
}