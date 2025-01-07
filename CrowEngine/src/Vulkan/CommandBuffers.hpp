#pragma once

#include "crow_lib.hpp"
#include "Vulkan/Device.hpp"
#include "Vulkan/QueueManager.hpp"

namespace vulkan
{
  class CommandBuffer
  {
  public:
    CommandBuffer(VkCommandBuffer* buffer) : buffer{buffer} {}

    void Begin();
    void End();
    void Reset()
    {
      vkResetCommandBuffer(*buffer, 0);
    }
  private:
    VkCommandBuffer* buffer;
    bool isRecording = false;
  };

  class CommandBufferManager {
  public:
    CommandBuffer CreateBuffer(QueueType bufferType)
    {
      CommandBuffer newBuffer(queueManager->GetCommandBuffer(bufferType));
    }

    void AddBuffer(CommandBuffer commandBuffer)
    {
      buffersResetPool.push_back(commandBuffer);
    }

    void RemoveBuffer(int index)
    {
      buffersResetPool.erase(buffersResetPool.begin() + index);
    }

    void resetPool() {
      for(CommandBuffer & buffer : buffersResetPool) {
        buffer.Reset();
      }
    }

  private:
    std::vector<CommandBuffer> buffersResetPool;
    std::unique_ptr<QueueManager>& queueManager;
    std::unique_ptr<Device>& device;
  };
}