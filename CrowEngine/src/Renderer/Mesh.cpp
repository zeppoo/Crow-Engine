#include "Renderer/Mesh.hpp"

#include "Vulkan/Vulkan_Module.hpp"

namespace renderer
{
  void Mesh::LoadData()
  {
    vertexBufferIndex = vulkan::VulkanModule::GetInstance().GetBufferManager()->CreateNewBuffer(VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, &vertices, vertices.size());
    indexBufferindex = vulkan::VulkanModule::GetInstance().GetBufferManager()->CreateNewBuffer(VK_BUFFER_USAGE_INDEX_BUFFER_BIT, &indices, indices.size());
  }

}