#pragma once
#include "crow_lib.hpp"
#include "glm/glm.hpp"

namespace vulkan
{

  enum BufferType
  {
    VERTEX = 0,
    INDEX,
    STORAGE,
    UNIFORM
  };

  struct Vertex
  {
    glm::vec2 position;
    glm::vec3 color;
  };

  struct VulkanBuffer
  {
    VkBuffer buffer;
    VkDeviceMemory memory;
    size_t size;
  };

  struct VertexBuffer
  {
    uint32_t handle;
    size_t size;
    std::vector<Vertex> vertices;
  };

  struct IndexBuffer
  {
    uint32_t handle;
    size_t count;  // number of indices
  };

  struct StorageBuffer
  {
    uint32_t handle;
    size_t size;
    uint32_t binding;  // shader binding point
  };

  struct UniformBuffer
  {
    uint32_t handle;
    size_t size;
    uint32_t binding;
  };
}