#include "VulkanGraphicsPipeline.hpp"

namespace crowe {

  VulkanGraphicsPipeline::VulkanGraphicsPipeline(std::unique_ptr<VulkanDevice>& device, std::unique_ptr<VulkanSwapChain>& swapchain) : device{device}, swapchain{swapchain}
  {
    createGraphicsPipeline();
  }

  VulkanGraphicsPipeline::~VulkanGraphicsPipeline(){}

  void VulkanGraphicsPipeline::createGraphicsPipeline(GraphicsPipelineCreateInfo graphicsPipelineCreateInfo)
  {

    // Creates ShaderModules/Infos
    VkShaderModule vertShader = CreateShaderModule(device->getDevice(), "../gfx/CompiledShaders/Crow.vert.spv");
    VkShaderModule fragShader = CreateShaderModule(device->getDevice(), "../gfx/CompiledShaders/Crow.frag.spv");
    VkPipelineShaderStageCreateInfo vertShaderInfo = CreateShaderStageInfo(VK_SHADER_STAGE_VERTEX_BIT, vertShader);
    VkPipelineShaderStageCreateInfo fragShaderInfo = CreateShaderStageInfo(VK_SHADER_STAGE_FRAGMENT_BIT, fragShader);
    VkPipelineShaderStageCreateInfo shaderStages[] = {vertShaderInfo, fragShaderInfo};

    GraphicsPipelineCreateInfo graphicsPipelineInfo{};

    VkGraphicsPipelineCreateInfo pipelineInfo{};
      pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
      pipelineInfo.stageCount = 2;
      pipelineInfo.pStages = shaderStages;
      pipelineInfo.pVertexInputState = &graphicsPipelineCreateInfo.vertexInputInfo;
      pipelineInfo.pInputAssemblyState = &graphicsPipelineCreateInfo.inputAssemblyInfo;
      pipelineInfo.pViewportState = &graphicsPipelineCreateInfo.viewportInfo;
      pipelineInfo.pRasterizationState = &graphicsPipelineCreateInfo.rasterizationInfo;
      pipelineInfo.pMultisampleState = &graphicsPipelineCreateInfo.multisampleInfo;
      pipelineInfo.pDepthStencilState = &graphicsPipelineCreateInfo.depthStencilInfo;
      pipelineInfo.pColorBlendState = &graphicsPipelineCreateInfo.colorBlendInfo;
      pipelineInfo.pDynamicState = &graphicsPipelineCreateInfo.dynamicInfo;
      pipelineInfo.layout = pipelineLayout;
      pipelineInfo.renderPass = swapchain->GetRenderPass();
      pipelineInfo.subpass = 0;
      pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

    if (vkCreateGraphicsPipelines(device->getDevice(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &graphicsPipeline) != VK_SUCCESS) {
      throw std::runtime_error("failed to create graphics pipeline!");
    }

    vkDestroyShaderModule(device->getDevice(), vertShader, nullptr);
    vkDestroyShaderModule(device->getDevice(), fragShader, nullptr);
  }

  void VulkanGraphicsPipeline::createPipelineLayout()
  {
    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 1;
    pipelineLayoutInfo.pSetLayouts = &descriptorSetLayout;

    if (vkCreatePipelineLayout(device->getDevice(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
    {
      throw std::runtime_error("failed to create pipeline layout!");
    }
  };

  void VulkanGraphicsPipeline::createDescriptorSetLayout() {
    VkDescriptorSetLayoutBinding uboLayoutBinding{};
    uboLayoutBinding.binding = 0;
    uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    uboLayoutBinding.descriptorCount = 1;
    uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
    uboLayoutBinding.pImmutableSamplers = nullptr;

    VkDescriptorSetLayoutCreateInfo layoutInfo{};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = 1;
    layoutInfo.pBindings = &uboLayoutBinding;

    if (vkCreateDescriptorSetLayout(device->getDevice(), &layoutInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS) {
      throw std::runtime_error("failed to create descriptor set layout!");
    }
  }

  void VulkanGraphicsPipeline::createDescriptorPool() {
    VkDescriptorPoolSize poolSize{};
    poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    poolSize.descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);

    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = 1;
    poolInfo.pPoolSizes = &poolSize;

    poolInfo.maxSets = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);

    if (vkCreateDescriptorPool(device->getDevice(), &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS) {
      throw std::runtime_error("failed to create descriptor pool!");
    }
  }

  void VulkanGraphicsPipeline::createDescriptorSets() {
    std::vector<VkDescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT, descriptorSetLayout);
    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = descriptorPool;
    allocInfo.descriptorSetCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
    allocInfo.pSetLayouts = layouts.data();

    descriptorSets.resize(MAX_FRAMES_IN_FLIGHT);
    if (vkAllocateDescriptorSets(device->getDevice(), &allocInfo, descriptorSets.data()) != VK_SUCCESS) {
      throw std::runtime_error("failed to allocate descriptor sets!");
    }

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
      VkDescriptorBufferInfo bufferInfo{};
      bufferInfo.buffer = uniformBuffers[i];
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

      vkUpdateDescriptorSets(device->getDevice(), 1, &descriptorWrite, 0, nullptr);
    }
  }
}