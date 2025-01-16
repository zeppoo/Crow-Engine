#include "Vulkan/BufferManager.hpp"

namespace vulkan
{
     BufferManager::BufferManager(std::shared_ptr<Device> device, std::shared_ptr<QueueManager> queueManager) : device{device}, queueManager{queueManager} {}

     BufferManager::~BufferManager()
    {

    }

    void BufferManager::CreateBuffer(BufferType bufferType, size_t dataSize)
    {
      VulkanBuffer buffer;
      buffer.size = dataSize;
      VkBufferCreateInfo bufferInfo{};
      bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
      bufferInfo.size = buffer.size;
      bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

      VkMemoryAllocateInfo allocInfo{};
      allocInfo.usage = VMA_MEMORY_USAGE_AUTO;  // Let VMA decide the best memory type

      switch (bufferType)
      {
        case VERTEX:
          bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
          buffers.push_back(buffer); // Handle, memory, size

          break;
        case INDEX:
          bufferInfo.usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
          buffers.push_back(buffer); // Handle, memory, size
          break;
        case STORAGE:
          bufferInfo.usage = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
          buffers.push_back(buffer); // Handle, memory, size
          break;
        case UNIFORM:
          bufferInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
          buffers.push_back(buffer); // Handle, memory, size
          break;
      }

      if (vkCreateBuffer(device->GetDevice(), &bufferInfo, nullptr, &buffer.buffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to create vertex buffer!");
      }

      VkMemoryRequirements memRequirements;
      vkGetBufferMemoryRequirements(device->GetDevice(), buffer.buffer, &memRequirements);
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