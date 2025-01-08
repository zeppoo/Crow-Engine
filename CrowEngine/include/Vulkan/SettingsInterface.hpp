#pragma once
#include "crow_lib.hpp"
#include "Config/RenderPassSettings.hpp"
#include "Config/PipelineSettings.hpp"
#include "Vulkan/BackendInterface.hpp"
namespace vulkan
{
  static VulkanModule* VLK = nullptr;

/*******************************************
*                                         *
*       GENERAL RESOURCE MANAGEMENT       *
*                                         *
*******************************************/
void RestartVulkan(); // Set up Vulkan resources needed for rendering.
void ShutDownVulkan(); // Release Vulkan resources at the end.
void BeginFrame(); // Prepare resources and begin the frame.
void ShowFrame(); // Present the frame and clean up temporary resources.
void ResizeWindow(int width, int height); // Handle window resizing.
void UpdateFrameBuffers(); // Recreate framebuffers if needed.
void UpdateBuffers(); // Recreate buffers if needed.
void UpdateDescriptorSets(); // Update descriptor sets for rendering.
void ReloadShaders(); // Reload shaders dynamically.
void SetShaderParameter(const char* name, float value); // Update a uniform parameter for shaders.
void CompileShader(const std::string& path); // Compile a new shader at runtime.
void UpdateImGuiSettings(); // Update settings controlled via ImGui.



/*******************************************
*                                         *
*        COMMANDBUFFER MANAGEMENT         *
*                                         *
*******************************************/
void RecordRenderBuffer();



/*******************************************
*                                         *
*          SWAPCHAIN MANAGEMENT           *
*                                         *
*******************************************/
void RecreateSwapchain(VkDevice device, VkSurfaceKHR surface, VkSwapchainKHR oldSwapchain, VkExtent2D newExtent); // Recreates the swapchain during resizing or format changes.
void PresentSwapchainImage(VkQueue queue, VkSwapchainKHR swapchain, uint32_t imageIndex, VkSemaphore waitSemaphore); // Submits the image to the presentation engine.



/*******************************************
*                                         *
*         RENDER PASS MANAGEMENT          *
*                                         *
*******************************************/
void CreateRenderPass(const RenderPassConfig& config); // Creates a new render pass with the given configuration.
void DestroyRenderPass(VkRenderPass renderPass); // Destroys a render pass to free resources.
void BeginRenderPass(VkCommandBuffer commandBuffer, VkRenderPass renderPass, VkFramebuffer framebuffer, const VkRect2D& renderArea, const VkClearValue* clearValues, uint32_t clearValueCount); // Begins a render pass.
void EndRenderPass(VkCommandBuffer commandBuffer); // Ends the currently active render pass.
void UpdateRenderPassAttachments(VkRenderPass renderPass, const std::vector<VkAttachmentDescription>& attachments); // Updates the attachments of an existing render pass.
void SetRenderPassDependency(VkRenderPass renderPass, const VkSubpassDependency& dependency); // Sets a subpass dependency for synchronization between subpasses.
void AddSubpassToRenderPass(VkRenderPass renderPass, const VkSubpassDescription& subpass); // Adds a new subpass to an existing render pass.



/*******************************************
*                                         *
*           SHADER MANAGEMENT             *
*                                         *
*******************************************/
void Play();
void Pause();
void Stop();



/*******************************************
*                                         *
*       GRAPHICS PIPELINE SETTINGS        *
*                                         *
*******************************************/
void CreateNewPipeline(); // Creates New Graphics Pipeline
void LoadPipeline();
void SetPipeline(VkPipeline newPipeline); // Binds a new pipeline for rendering.
void UpdatePipelineLayout(VkPipelineLayout newLayout); // Updates the pipeline layout, affecting descriptor sets and push constants.
void SetViewport(const VkViewport& viewport); // Sets a new viewport dynamically.
void SetScissor(const VkRect2D& scissor); // Sets a new scissor rectangle dynamically.
void SetCullMode(VkCullModeFlags cullMode); // Updates the cull mode (e.g., front-face, back-face, none).
void SetFrontFace(VkFrontFace frontFace); // Changes the front face winding order (clockwise or counter-clockwise).
void SetBlendState(bool enable, VkBlendFactor srcFactor, VkBlendFactor dstFactor, VkBlendOp blendOp); // Configures blending state for color attachment.
void SetDepthTest(bool enable, VkCompareOp compareOp); // Toggles depth testing and sets the comparison operator.
void SetStencilTest(bool enable, VkStencilOpState frontState, VkStencilOpState backState); // Configures stencil testing for the pipeline.
void UpdateDynamicStates(const std::vector<VkDynamicState>& states); // Updates the dynamic state list in the pipeline.
void UpdateVertexShader(const std::string& shaderPath); // Replace the vertex shader module in the pipeline.
void UpdateFragmentShader(const std::string& shaderPath); // Replace the fragment shader module in the pipeline.
void ReloadPipelineShaders(); // Reloads all shaders for the pipeline from their respective files.
void RebuildPipeline(VkRenderPass renderPass, VkPipelineLayout pipelineLayout, const PipelineConfig& config); // Rebuilds the graphics pipeline with updated configurations.
void SwitchToWireframeMode(); // Switches the polygon mode to wireframe.
void SetRasterizationState(const VkPipelineRasterizationStateCreateInfo& rasterizationState); // Updates the rasterization state (e.g., depth bias, polygon mode).
void SetColorBlendAttachment(const VkPipelineColorBlendAttachmentState& blendState); // Updates blending state for a specific color attachment.
void SetDepthStencilState(const VkPipelineDepthStencilStateCreateInfo& depthStencilState); // Configures the depth and stencil state.
void SwitchToSolidFillMode(); // Switches the polygon mode to solid fill.
void SetPrimitiveTopology(VkPrimitiveTopology topology); // Changes the primitive topology (e.g., triangle list, line list).
void EnableMultiSample(uint32_t sampleCount); // Configures multisampling in the pipeline.
void UpdateDescriptorSet(VkDescriptorSet descriptorSet); // Re-binds a descriptor set to the pipeline.
void PushConstants(VkPipelineLayout pipelineLayout, VkShaderStageFlags stageFlags, uint32_t offset, uint32_t size, const void* data); // Updates push constants for a specific shader stage.
void BindTextureToDescriptor(VkDescriptorSet descriptorSet, uint32_t binding, VkImageView imageView, VkSampler sampler); // Binds a texture to a descriptor set at a specific binding.
void SetReflection(PipelineSettings& settings);
void SetVertexBindingDescriptionCount(PipelineSettings& settings, uint32_t count);
void SetVertexAttributeDescriptionCount(PipelineSettings& settings, uint32_t count);
void SetTopology(PipelineSettings& settings, VkPrimitiveTopology topology);
void SetPrimitiveRestartEnable(PipelineSettings& settings, VkBool32 enable);
void SetPatchControlPoints(PipelineSettings& settings, uint32_t points);
void SetViewportCount(PipelineSettings& settings, uint32_t count);
void SetScissorCount(PipelineSettings& settings, uint32_t count);
void SetDepthClampEnable(PipelineSettings& settings, VkBool32 enable);
void SetRasterizerDiscardEnable(PipelineSettings& settings, VkBool32 enable);
void SetPolygonMode(PipelineSettings& settings, VkPolygonMode mode);
void SetCullMode(PipelineSettings& settings, VkCullModeFlags mode);
void SetFrontFace(PipelineSettings& settings, VkFrontFace face);
void SetDepthBiasEnable(PipelineSettings& settings, VkBool32 enable);
void SetDepthBiasConstantFactor(PipelineSettings& settings, float factor);
void SetDepthBiasClamp(PipelineSettings& settings, float clamp);
void SetDepthBiasSlopeFactor(PipelineSettings& settings, float slope);
void SetLineWidth(PipelineSettings& settings, float width);
void SetRasterizationSamples(PipelineSettings& settings, VkSampleCountFlagBits samples);
void SetSampleShadingEnable(PipelineSettings& settings, VkBool32 enable);
void SetMinSampleShading(PipelineSettings& settings, float shading);
void SetSampleMask(PipelineSettings& settings, VkSampleMask mask);
void SetAlphaToCoverageEnable(PipelineSettings& settings, VkBool32 enable);
void SetAlphaToOneEnable(PipelineSettings& settings, VkBool32 enable);
void SetDepthTestEnable(PipelineSettings& settings, VkBool32 enable);
void SetDepthWriteEnable(PipelineSettings& settings, VkBool32 enable);
void SetDepthCompareOp(PipelineSettings& settings, VkCompareOp op);
void SetDepthBoundsTestEnable(PipelineSettings& settings, VkBool32 enable);
void SetStencilTestEnable(PipelineSettings& settings, VkBool32 enable);
void SetBlendEnable(PipelineSettings& settings, VkBool32 enable);
void SetSrcColorBlendFactor(PipelineSettings& settings, VkBlendFactor factor);
void SetDstColorBlendFactor(PipelineSettings& settings, VkBlendFactor factor);
void SetColorBlendOp(PipelineSettings& settings, VkBlendOp op);
void SetSrcAlphaBlendFactor(PipelineSettings& settings, VkBlendFactor factor);
void SetDstAlphaBlendFactor(PipelineSettings& settings, VkBlendFactor factor);
void SetAlphaBlendOp(PipelineSettings& settings, VkBlendOp op);
void SetColorWriteMask(PipelineSettings& settings, VkColorComponentFlags mask);
void SetLogicOpEnable(PipelineSettings& settings, VkBool32 enable);
void SetLogicOp(PipelineSettings& settings, VkLogicOp op);
void SetAttachmentCount(PipelineSettings& settings, uint32_t count);
void SetBlendConstants(PipelineSettings& settings, const float constants[4]);
void SetDynamicStates(PipelineSettings& settings, const std::vector<VkDynamicState>& states);



/*******************************************
*                                         *
*        DEBUGGING AND UTILITIES          *
*                                         *
*******************************************/
}
