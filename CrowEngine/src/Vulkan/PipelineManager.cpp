#include "Vulkan/PipelineManager.hpp"
#include "Logger.hpp"
#include "ConfigPaths.hpp"

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
    newPipeline.InitializePipelineLayout(device->getDevice());
    newPipeline.InitializePipeline(device->getDevice(), swapchain->GetRenderPass(), pipelineInfo);
    graphicsPipelines.push_back(newPipeline);
  }

  void PipelineManager::BindPipeline(VkCommandBuffer* pCommandBuffer, GraphicsPipeline &graphicsPipeline)
  {
    vkCmdBindPipeline(*pCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline.pipeline);

    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = static_cast<float>(swapchain->GetSwapchainExtent().width);
    viewport.height = static_cast<float>(swapchain->GetSwapchainExtent().height);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vkCmdSetViewport(*pCommandBuffer, 0, 1, &viewport);

    VkRect2D scissor{};
    scissor.offset = {0, 0};
    scissor.extent = swapchain->GetSwapchainExtent();
    vkCmdSetScissor(*pCommandBuffer, 0, 1, &scissor);

    vkCmdDraw(*pCommandBuffer, 3, 1, 0, 0);
  }


}