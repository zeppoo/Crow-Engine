#pragma once
#define GLFW_INCLUDE_VULKAN

#include "vulkan/vulkan.h"
#include "glm/glm.hpp"
#include <iostream>
#include <vector>
#include <memory>
#include <string>

namespace vulkan
{

struct Vertex
{
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec2 uv;
};

class Texture
{
  VkImage image;
  VkDeviceMemory mem;
  VkImageView view;
};

class Model {
  std::vector<Vertex> vertices;        // Position, UV, Normal, etc
  std::vector<uint32_t> indices;       // Index buffer data

  VkBuffer* vertexBuffer;
  VkBuffer* indexBuffer;
  VkDeviceMemory* vertexBufferMemory;
  VkDeviceMemory* indexBufferMemory;
};

class Buffer {
public:
  VkBuffer buffer = VK_NULL_HANDLE;
  VkDeviceMemory memory = VK_NULL_HANDLE;
  VkDeviceSize size = 0;
  VkMemoryPropertyFlags memoryProperties;
  bool createStaging = false;

  ~Buffer() = default;

  void create();
  void destroy();
  void update(const void* data, VkDeviceSize size, VkDeviceSize offset = 0);
};
}