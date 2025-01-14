#include "Vulkan/PipelineManager.hpp"
#include "Logger.hpp"
#include "ConfigPaths.hpp"
#include "Vulkan/FrameManager.hpp"

namespace vulkan
{
  void GraphicsPipeline::InitializePipelineLayout(const VkDevice &device)
  {
    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 0; // Optional
    pipelineLayoutInfo.pSetLayouts = nullptr; // Optional
    pipelineLayoutInfo.pushConstantRangeCount = 0; // Optional
    pipelineLayoutInfo.pPushConstantRanges = nullptr; // Optional


    if (vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, &this->pipelineLayout) != VK_SUCCESS) {
      throw std::runtime_error("failed to create pipeline layout!");
    }
  }

  void GraphicsPipeline::InitializePipeline(const VkDevice& device, const VkRenderPass& renderPass, const PipelineInfo pipelineInfo)
  {
    VkShaderModule vertShader = CreateShaderModule(device, default_vert_spv);
    VkShaderModule fragShader = CreateShaderModule(device, default_frag_spv);
    VkPipelineShaderStageCreateInfo vertShaderInfo = CreateShaderStageInfo(VK_SHADER_STAGE_VERTEX_BIT, vertShader);
    VkPipelineShaderStageCreateInfo fragShaderInfo = CreateShaderStageInfo(VK_SHADER_STAGE_FRAGMENT_BIT, fragShader);
    VkPipelineShaderStageCreateInfo shaderStages[] = {vertShaderInfo, fragShaderInfo};

    VkGraphicsPipelineCreateInfo pipelineCreateInfo{};
    pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineCreateInfo.stageCount = 2;
    pipelineCreateInfo.pStages = shaderStages;
    pipelineCreateInfo.pVertexInputState = &pipelineInfo.vertexInputInfo;
    pipelineCreateInfo.pInputAssemblyState = &pipelineInfo.inputAssemblyInfo;
    pipelineCreateInfo.pViewportState = &pipelineInfo.viewportInfo;
    pipelineCreateInfo.pRasterizationState = &pipelineInfo.rasterizationInfo;
    pipelineCreateInfo.pMultisampleState = &pipelineInfo.multisampleInfo;
    pipelineCreateInfo.pColorBlendState = &pipelineInfo.colorBlendInfo;
    pipelineCreateInfo.pDynamicState = &pipelineInfo.dynamicStatesInfo;
    pipelineCreateInfo.layout = this->pipelineLayout;
    pipelineCreateInfo.renderPass = renderPass;
    pipelineCreateInfo.subpass = 0;
    pipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;

    if (vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineCreateInfo, nullptr, &this->pipeline) !=
        VK_SUCCESS) {
      throw std::runtime_error("failed to create graphics pipeline!");
    }

    logger::Info("Successfully Created GraphicsPipeline");

    vkDestroyShaderModule(device, vertShader, nullptr);
    vkDestroyShaderModule(device, fragShader, nullptr);
  }

  void GraphicsPipeline::InitializeDescriptorLayout(const VkDevice& device)
  {
    VkDescriptorSetLayoutBinding uboLayoutBinding{};
    uboLayoutBinding.binding = 0;
    uboLayoutBinding.descriptorCount = 1;
    uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    uboLayoutBinding.pImmutableSamplers = nullptr;
    uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

    VkDescriptorSetLayoutCreateInfo layoutInfo{};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = 1;
    layoutInfo.pBindings = &uboLayoutBinding;

    if (vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS) {
      throw std::runtime_error("failed to create descriptor set layout!");
    }
  }

  void GraphicsPipeline::InitializeDescriptorPool(const VkDevice& device, std::vector<VkDescriptorPoolSize> poolSizes)
  {
    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = 1;
    poolInfo.pPoolSizes = poolSizes.data();
    poolInfo.maxSets = static_cast<uint32_t>(FRAMES_IN_FLIGHT);

    if (vkCreateDescriptorPool(device, &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS) {
      throw std::runtime_error("failed to create descriptor pool!");
    }
  }

  void GraphicsPipeline::InitializeDescriptorSets(const VkDevice& device, std::vector<Buffer> buffers)
  {
    std::vector<VkDescriptorSetLayout> layouts(FRAMES_IN_FLIGHT, descriptorSetLayout);
    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = descriptorPool;
    allocInfo.descriptorSetCount = static_cast<uint32_t>(FRAMES_IN_FLIGHT);
    allocInfo.pSetLayouts = layouts.data();

    descriptorSets.resize(FRAMES_IN_FLIGHT);
    if (vkAllocateDescriptorSets(device, &allocInfo, descriptorSets.data()) != VK_SUCCESS) {
      throw std::runtime_error("failed to allocate descriptor sets!");
    }

    for (size_t i = 0; i < buffers.size(); i++) {
      VkDescriptorBufferInfo bufferInfo{};
      bufferInfo.buffer = buffers[i].buffer;
      bufferInfo.offset = 0;
      bufferInfo.range = sizeof(UniformBufferObject);

      VkWriteDescriptorSet descriptorWrite{};
      descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
      descriptorWrite.dstSet = descriptorSets[i];
      descriptorWrite.dstBinding = 0;
      descriptorWrite.dstArrayElement = 0;
      descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
      descriptorWrite.descriptorCount = 1;
      descriptorWrite.pBufferInfo = &bufferInfo;

      vkUpdateDescriptorSets(device, 1, &descriptorWrite, 0, nullptr);
    }
  }

  PipelineManager::PipelineManager(std::unique_ptr<Device> &device, std::unique_ptr<SwapChain> &swapChain, std::unique_ptr<FrameManager> &frameManager) : device{device}, swapchain{swapChain}, frameManager{frameManager} {
    SetupPipelineManager();
  }

  void PipelineManager::SetupPipelineManager()
  {
    PipelineSettings pipelineSettings{};
    PipelineInfo pipelineInfo;
    pipelineInfo = CreatePipelineInfo(pipelineSettings, swapchain->GetSwapchainExtent());
    CreateGraphicsPipeline(pipelineInfo);
  }

  void PipelineManager::CreateGraphicsPipeline(PipelineInfo pipelineInfo)
  {
    logger::Info("Creating new Pipeline...");
    GraphicsPipeline newPipeline{};
    newPipeline.InitializePipelineLayout(device->GetDevice());
    newPipeline.InitializePipeline(device->GetDevice(), swapchain->GetRenderPasses()[0], pipelineInfo);
    graphicsPipelines.push_back(newPipeline);
  }

  void GraphicsPipeline::BindPipeline(VkCommandBuffer* pCommandBuffer)
  {
    vkCmdBindPipeline(*pCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
  }


}