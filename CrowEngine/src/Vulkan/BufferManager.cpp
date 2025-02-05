#include "Vulkan/BufferManager.hpp"

#include "Vulkan/Vulkan_Types.hpp"

namespace vulkan
{
     BufferManager::BufferManager(std::shared_ptr<Device> device, std::shared_ptr<CommandBufferManager> cmdBufferManager) : device{device}, cmdBufferManager{cmdBufferManager} {}

     BufferManager::~BufferManager()
    {
      for (const auto& buffer : buffers){
        vkDestroyBuffer(device->GetDevice(), buffer.buffer, nullptr);
        vkFreeMemory(device->GetDevice(), buffer.memory, nullptr);
      }
    }

    int BufferManager::CreateNewBuffer(VkBufferUsageFlagBits usage, void* data, size_t size)
    {
      Buffer newBuffer{};

       VkBufferCreateInfo bufferInfo{};
       bufferInfo.size = size;
       bufferInfo.usage = usage; // Or other usage flags
       bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

       vkCreateBuffer(device->GetDevice(), &bufferInfo, nullptr, &newBuffer.buffer);

      if (vkCreateBuffer(device->GetDevice(), &bufferInfo, nullptr, &newBuffer.buffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to create vertex buffer!");
      }

      VkMemoryRequirements memRequirements;
      vkGetBufferMemoryRequirements(device->GetDevice(), newBuffer.buffer, &memRequirements);

      VkMemoryAllocateInfo allocInfo{};
      allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
      allocInfo.allocationSize = memRequirements.size;
      allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

      if (vkAllocateMemory(device->GetDevice(), &allocInfo, nullptr, &newBuffer.memory) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate vertex buffer memory!");
      }

      if (vkBindBufferMemory(device->GetDevice(), newBuffer.buffer, newBuffer.memory, 0)) {
        std::cout << "Buffer created successfully" << std::endl;
      } else {
        std::cout << "Buffer already exists" << std::endl;
      }

       buffers.push_back(newBuffer);
       return buffers.size() - 1;
    }

    void BufferManager::CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) {
      CommandBuffer commandBuffer = cmdBufferManager->CreateBuffer(TRANSFER, 1);
      commandBuffer.BeginRecording();

      VkBufferCopy copyRegion{};
      copyRegion.size = size;
      vkCmdCopyBuffer(*commandBuffer.buffer, srcBuffer, dstBuffer, 1, &copyRegion);

      commandBuffer.EndRecording();
       cmdBufferManager->QueueBuffer(commandBuffer, 1);
       cmdBufferManager->SubmitBuffers(TRANSFER, 1);
    }

    void BufferManager::DestroyBuffer(const int index)
    {
      vkDestroyBuffer(device->GetDevice(), buffers[index].buffer, nullptr);
      vkFreeMemory(device->GetDevice(), buffers[index].memory, nullptr);
       buffers.erase(buffers.begin() + index);
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