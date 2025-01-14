#include "Vulkan/CommandBuffers.hpp"

namespace vulkan {
  CommandBuffer::CommandBuffer(VkCommandBuffer* buffer, QueueType type)
      : buffer{buffer}, bufferType{type}{}

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

  CommandBufferManager::CommandBufferManager(std::shared_ptr<Device> device, std::shared_ptr<QueueManager> queueManager) : device{device}, queueManager{std::move(queueManager)} {}


  CommandBuffer CommandBufferManager::CreateBuffer(QueueType bufferType) {

    CommandBuffer newBuffer(queueManager->GetCommandBuffer(bufferType, 1),bufferType);
    return newBuffer;
  }

  void CommandBufferManager::QueueBuffer(CommandBuffer commandBuffer) {
    if(!commandBuffer.isRecorded)
      return logger::Error("Cannot submit commandbuffer because it is not recorded");
    queueManager->QueueCommandBuffer(commandBuffer.buffer, commandBuffer.bufferType, 1);
  }

  void CommandBufferManager::SubmitBuffers(QueueType queueType, uint32_t queueIndex)
  {
    queueManager->SubmitQueuedBuffers(device->GetDevice(), queueType, queueIndex);
  }
}