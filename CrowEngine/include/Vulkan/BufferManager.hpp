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
  struct Buffer {
    std::string name;
    std::vector<VkBuffer> buffers = {};
    VkBufferCreateInfo bufferInfo{};
    VkDeviceMemory bufferMemory = VK_NULL_HANDLE;

    virtual VkBufferCreateInfo* createBufferInfo();

    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
  };

  struct VertexBuffer : Buffer {
    const std::vector<std::vector<Vertex>> vertices;

    VkBufferCreateInfo* createBufferInfo() override;

    void BindVertexBuffer(VkCommandBuffer* pCommandBuffer);
  };

  struct IndexBuffer : Buffer {
    const std::vector<uint16_t> indices;

    VkBufferCreateInfo* createBufferInfo() override;

    void BindIndexBuffer(VkCommandBuffer* pCommandBuffer);
  };

  struct UniformBuffer : Buffer {

    VkBufferCreateInfo* createBufferInfo() override;
  };

  class BufferManager
  {
  public:
    BufferManager(std::unique_ptr<Device> &device, std::unique_ptr<QueueManager> &queueManager);
    ~BufferManager();

    void AddCommandBuffer(const std::string name, QueueType type);
    void DestroyCommandBuffer(const std::string name);

    void CreateNewBuffer(const std::string name, Buffer& buffer);
    void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
    void DestroyBuffer(const std::string name);

  private:
    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

    std::unique_ptr<QueueManager> &queueManager;
    std::unique_ptr<Device> &device;

    std::unordered_map<std::string, Buffer> buffers;
  };
}