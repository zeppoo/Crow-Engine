#pragma once
#include "Logger.hpp"
#include "Vulkan/Device.hpp"
#include "Vulkan/QueueManager.hpp"
#include "crow_lib.hpp"

namespace vulkan
{
struct CommandBuffer {
  CommandBuffer(VkCommandBuffer *buffer, QueueType type);
  ~CommandBuffer();

  void BeginRecording();
  void EndRecording();
  void Reset();

  std::shared_ptr<QueueManager> queueManager;
  VkCommandBuffer *buffer;
  QueueType bufferType;
  bool isRecorded = false;
};

class CommandBufferManager {
public:
  CommandBufferManager(std::shared_ptr<Device> device, std::shared_ptr<QueueManager> queueManager);

  CommandBuffer CreateBuffer(QueueType bufferType);
  void QueueBuffer(CommandBuffer commandBuffer);
  void SubmitBuffers(QueueType bufferType, uint32_t queueIndex);

private:
  std::shared_ptr<Device> device;
  std::shared_ptr<QueueManager> queueManager;
};
} // namespace vulkan
