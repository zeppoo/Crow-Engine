#include "Vulkan/SettingsInterface.hpp"

namespace vulkan
{
/*******************************************
*                                         *
*       GENERAL RESOURCE MANAGEMENT       *
*                                         *
*******************************************/



/*******************************************
*                                         *
*        SWAPCHAIN MANAGEMENT             *
*                                         *
*******************************************/



/*******************************************
*                                         *
*       RENDER PASS MANAGEMENT            *
*                                         *
*******************************************/
void CreateRenderPass(const RenderPassConfig &config)
{

}

void RemoveRenderPass(const RenderPassConfig &config)
{

}

/*******************************************
*                                         *
*           SHADER MANAGEMENT             *
*                                         *
*******************************************/
void ReloadShaders()
{

}

void CompileShader(const std::string& path)
{

}

void Play()
{
  module->BeginShaderExecution();
}

void Pause()
{

}

void Stop()
{
  module->EndShaderExecution();
}



/*******************************************
*                                         *
*       GRAPHICS PIPELINE SETTINGS        *
*                                         *
*******************************************/
void CreateNewGraphicsPipeline()
{
  module->CreateNewGraphicsPipeline();
}

void RemoveGraphicsPipeline()
{
 module.
}

// Vertex Input
void SetVertexBindingDescriptionCount(PipelineSettings& settings, uint32_t count) {
    settings.vertexBindingDescriptionCount = count;
}

void SetVertexAttributeDescriptionCount(PipelineSettings& settings, uint32_t count) {
    settings.vertexAttributeDescriptionCount = count;
}

// Input Assembly
void SetTopology(PipelineSettings& settings, VkPrimitiveTopology topology) {
    settings.topology = topology;
}

void SetPrimitiveRestartEnable(PipelineSettings& settings, VkBool32 enable) {
    settings.primitiveRestartEnable = enable;
}

// Tessellation
void SetPatchControlPoints(PipelineSettings& settings, uint32_t points) {
    settings.patchControlPoints = points;
}

// Viewports and Scissors
void SetViewportCount(PipelineSettings& settings, uint32_t count) {
    settings.viewportCount = count;
}

void SetScissorCount(PipelineSettings& settings, uint32_t count) {
    settings.scissorCount = count;
}

// Rasterizer
void SetDepthClampEnable(PipelineSettings& settings, VkBool32 enable) {
    settings.depthClampEnable = enable;
}

void SetRasterizerDiscardEnable(PipelineSettings& settings, VkBool32 enable) {
    settings.rasterizerDiscardEnable = enable;
}

void SetPolygonMode(PipelineSettings& settings, VkPolygonMode mode) {
    settings.polygonMode = mode;
}

void SetCullMode(PipelineSettings& settings, VkCullModeFlags mode) {
    settings.cullMode = mode;
}

void SetFrontFace(PipelineSettings& settings, VkFrontFace face) {
    settings.frontFace = face;
}

void SetDepthBiasEnable(PipelineSettings& settings, VkBool32 enable) {
    settings.depthBiasEnable = enable;
}

void SetDepthBiasConstantFactor(PipelineSettings& settings, float factor) {
    settings.depthBiasConstantFactor = factor;
}

void SetDepthBiasClamp(PipelineSettings& settings, float clamp) {
    settings.depthBiasClamp = clamp;
}

void SetDepthBiasSlopeFactor(PipelineSettings& settings, float slope) {
    settings.depthBiasSlopeFactor = slope;
}

void SetLineWidth(PipelineSettings& settings, float width) {
    settings.lineWidth = width;
}

// Multisampling
void SetRasterizationSamples(PipelineSettings& settings, VkSampleCountFlagBits samples) {
    settings.rasterizationSamples = samples;
}

void SetSampleShadingEnable(PipelineSettings& settings, VkBool32 enable) {
    settings.sampleShadingEnable = enable;
}

void SetMinSampleShading(PipelineSettings& settings, float shading) {
    settings.minSampleShading = shading;
}

void SetSampleMask(PipelineSettings& settings, VkSampleMask mask) {
    settings.sampleMask = mask;
}

void SetAlphaToCoverageEnable(PipelineSettings& settings, VkBool32 enable) {
    settings.alphaToCoverageEnable = enable;
}

void SetAlphaToOneEnable(PipelineSettings& settings, VkBool32 enable) {
    settings.alphaToOneEnable = enable;
}

// Depth and Stencil Testing
void SetDepthTestEnable(PipelineSettings& settings, VkBool32 enable) {
    settings.depthTestEnable = enable;
}

void SetDepthWriteEnable(PipelineSettings& settings, VkBool32 enable) {
    settings.depthWriteEnable = enable;
}

void SetDepthCompareOp(PipelineSettings& settings, VkCompareOp op) {
    settings.depthCompareOp = op;
}

void SetDepthBoundsTestEnable(PipelineSettings& settings, VkBool32 enable) {
    settings.depthBoundsTestEnable = enable;
}

void SetStencilTestEnable(PipelineSettings& settings, VkBool32 enable) {
    settings.stencilTestEnable = enable;
}

// Color Blending Attachment
void SetBlendEnable(PipelineSettings& settings, VkBool32 enable) {
    settings.blendEnable = enable;
}

void SetSrcColorBlendFactor(PipelineSettings& settings, VkBlendFactor factor) {
    settings.srcColorBlendFactor = factor;
}

void SetDstColorBlendFactor(PipelineSettings& settings, VkBlendFactor factor) {
    settings.dstColorBlendFactor = factor;
}

void SetColorBlendOp(PipelineSettings& settings, VkBlendOp op) {
    settings.colorBlendOp = op;
}

void SetSrcAlphaBlendFactor(PipelineSettings& settings, VkBlendFactor factor) {
    settings.srcAlphaBlendFactor = factor;
}

void SetDstAlphaBlendFactor(PipelineSettings& settings, VkBlendFactor factor) {
    settings.dstAlphaBlendFactor = factor;
}

void SetAlphaBlendOp(PipelineSettings& settings, VkBlendOp op) {
    settings.alphaBlendOp = op;
}

void SetColorWriteMask(PipelineSettings& settings, VkColorComponentFlags mask) {
    settings.colorWriteMask = mask;
}

// Color Blending
void SetLogicOpEnable(PipelineSettings& settings, VkBool32 enable) {
    settings.logicOpEnable = enable;
}

void SetLogicOp(PipelineSettings& settings, VkLogicOp op) {
    settings.logicOp = op;
}

void SetAttachmentCount(PipelineSettings& settings, uint32_t count) {
    settings.attachmentCount = count;
}

void SetBlendConstants(PipelineSettings& settings, const float constants[4]) {
    for (int i = 0; i < 4; ++i) {
        settings.blendConstants[i] = constants[i];
    }
}

// Dynamic states
void SetDynamicStates(PipelineSettings& settings, const std::vector<VkDynamicState>& states) {
    settings.dynamicStates = states;
}

/*******************************************
*                                         *
*       DEBUGGING AND UTILITIES           *
*                                         *
*******************************************/
}