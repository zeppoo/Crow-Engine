#include "Vulkan/BufferManager.hpp"

namespace vulkan
{
    VkBufferCreateInfo* Buffer::createBufferInfo()
    {
      return nullptr;
    }

    VkBufferCreateInfo* VertexBuffer::createBufferInfo()
    {
      bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
      bufferInfo.size = sizeof(vertices[0]) * vertices.size();
      bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
      bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

      return &bufferInfo;
    }

    void VertexBuffer::BindVertexBuffer(VkCommandBuffer* pCommandBuffer)
    {
      VkDeviceSize offsets[] = {0};
      vkCmdBindVertexBuffers(*pCommandBuffer, 0, 1, buffers.data(), offsets);
    }

    VkBufferCreateInfo* IndexBuffer::createBufferInfo()
    {
      bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
      bufferInfo.size = sizeof(indices[0]) * indices.size();
      bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
      bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

      return &bufferInfo;
    }

    void IndexBuffer::BindIndexBuffer(VkCommandBuffer *pCommandBuffer)
    {
      vkCmdBindIndexBuffer(*pCommandBuffer, buffer, 0, VK_INDEX_TYPE_UINT16);

      vkCmdDrawIndexed(*pCommandBuffer, static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);
    }

     BufferManager::BufferManager(std::unique_ptr<Device> &device, std::unique_ptr<QueueManager> &queueManager) : device{device}, queueManager{queueManager} {}

     BufferManager::~BufferManager()
    {
      for (const auto& pair : buffers){
        vkDestroyBuffer(device->GetDevice(), pair.second.buffer, nullptr);
        vkFreeMemory(device->GetDevice(), pair.second.bufferMemory, nullptr);
      }
    }

    void BufferManager::CreateNewBuffer(const std::string name, Buffer& buffer)
    {
      if (vkCreateBuffer(device->GetDevice(), buffer.createBufferInfo(), nullptr, &buffer.buffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to create vertex buffer!");
      }

      VkMemoryRequirements memRequirements;
      vkGetBufferMemoryRequirements(device->GetDevice(), buffer.buffer, &memRequirements);

      VkMemoryAllocateInfo allocInfo{};
      allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
      allocInfo.allocationSize = memRequirements.size;
      allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

      if (vkAllocateMemory(device->GetDevice(), &allocInfo, nullptr, &buffer.bufferMemory) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate vertex buffer memory!");
      }

      vkBindBufferMemory(device->GetDevice(), buffer.buffer, buffer.bufferMemory, 0);
      buffer.name = name;

      auto result = buffers.insert({name, buffer});

      if (result.second) {
        std::cout << "Buffer created successfully" << std::endl;
      } else {
        std::cout << "Buffer already exists" << std::endl;
      }
    }

    void BufferManager::CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) {
      VkCommandBuffer* pCommandBuffer = queueManager->GetCommandBuffer(TRANSFER);
      VkCommandBufferBeginInfo beginInfo{};
      beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
      beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

      vkBeginCommandBuffer(*pCommandBuffer, &beginInfo);

      VkBufferCopy copyRegion{};
      copyRegion.size = size;
      vkCmdCopyBuffer(*pCommandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

      vkEndCommandBuffer(*pCommandBuffer);

      VkSubmitInfo submitInfo{};
      submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
      submitInfo.commandBufferCount = 1;
      submitInfo.pCommandBuffers = pCommandBuffer;

      vkQueueSubmit(*queueManager->GetTransferQueues()[0].pQueue, 1, &submitInfo, VK_NULL_HANDLE);
      vkQueueWaitIdle(*queueManager->GetTransferQueues()[0].pQueue);

      vkResetCommandBuffer(*pCommandBuffer, 0);
    }

    void BufferManager::DestroyBuffer(const std::string name)
    {
      vkDestroyBuffer(device->GetDevice(), buffers[name].buffer, nullptr);
      vkFreeMemory(device->GetDevice(), buffers[name].bufferMemory, nullptr);
      buffers.erase(name);
    }

    uint32_t BufferManager::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) {
      VkPhysicalDeviceMemoryProperties memProperties;
      vkGetPhysicalDeviceMemoryProperties(device->GetPhysicDevice(), &memProperties);

      for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
        if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
          return i;
        }
      }
      throw std::runtime_error("failed to find suitable memory type!");
    }

}