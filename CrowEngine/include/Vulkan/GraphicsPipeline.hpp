#pragma once

#include "crow_lib.hpp"
#include "Device.hpp"
#include "SwapChain.hpp"

namespace vulkan
{
  class GraphicsPipeline {
  public:
    GraphicsPipeline(std::unique_ptr<Device> &device, std::unique_ptr<SwapChain> &swapChain);

    ~GraphicsPipeline();

  private:

    void CreatePipelineLayout();

    void createDescriptorSetLayout();

    std::unique_ptr<Device> &device;
    std::unique_ptr<SwapChain> &swapchain;
  };
}
