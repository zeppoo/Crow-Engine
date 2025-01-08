#include "Vulkan/CommandBuffers.hpp"

namespace vulkan {
  CommandBuffer::CommandBuffer(VkCommandBuffer* buffer, QueueType type)
      : buffer{buffer}, bufferType{type} {}

  CommandBuffer::~CommandBuffer() {
    vkResetCommandBuffer(*buffer, 0);
  }

  void CommandBuffer::BeginRecording() {
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    vkBeginCommandBuffer(*buffer, &beginInfo);
  }

  void CommandBuffer::EndRecording() {
    vkEndCommandBuffer(*buffer);
    isRecorded = true;
  }

  void CommandBuffer::Reset() {
    vkResetCommandBuffer(*buffer, 0);
    isRecorded = false;
  }

  CommandBufferManager::CommandBufferManager(std::unique_ptr<Device>& device, std::unique_ptr<QueueManager>& queueManager) : device{device}, queueManager{queueManager} {}


  CommandBuffer CommandBufferManager::CreateBuffer(QueueType bufferType) {
    CommandBuffer newBuffer(queueManager->GetCommandBuffer(bufferType), bufferType);
    return newBuffer;
  }

  void CommandBufferManager::QueueBuffer(CommandBuffer commandBuffer) {
    if(!commandBuffer.isRecorded)
      return logger::Error("Cannot submit commandbuffer because it is not recorded");
    queueManager->SubmitCommandBuffer(commandBuffer.buffer, commandBuffer.bufferType);
  }

  void CommandBufferManager::AddBuffer(CommandBuffer commandBuffer) {
    reuseBuffers.push_back(commandBuffer);
  }

  void CommandBufferManager::RemoveBuffer(int index) {
    reuseBuffers.erase(reuseBuffers.begin() + index);
  }

  void CommandBufferManager::ResetStoredBuffers() {
    for(CommandBuffer & buffer : reuseBuffers) {
      buffer.Reset();
    }
  }

  void CommandBufferManager::SubmitStoredBuffers() {
    for(CommandBuffer & buffer : reuseBuffers) {
      queueManager->SubmitCommandBuffer(buffer.buffer, buffer.bufferType);
    }
  }
}