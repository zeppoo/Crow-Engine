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

  VkCommandBuffer *buffer;
  QueueType bufferType;
  bool isRecorded = false;
};

class CommandBufferManager {
public:
  CommandBufferManager(std::unique_ptr<Device> &device, std::unique_ptr<QueueManager> &queueManager);

  CommandBuffer CreateBuffer(QueueType bufferType);
  void QueueBuffer(CommandBuffer commandBuffer);
  void AddBuffer(CommandBuffer commandBuffer);
  void RemoveBuffer(int index);
  void ResetStoredBuffers();
  void SubmitStoredBuffers();

private:
  std::vector<CommandBuffer> reuseBuffers;
  std::unique_ptr<Device> &device;
  std::unique_ptr<QueueManager> &queueManager;
};
} // namespace vulkan
