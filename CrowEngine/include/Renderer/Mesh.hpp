#pragma once
#include "Vulkan/Vulkan_Types.hpp"

namespace renderer
{
  class Mesh {
  public:
    int vertexBufferIndex;
    int indexBufferindex;

    std::vector<vulkan::Vertex> vertices;
    std::vector<uint32_t> indices;

    // Load/Set data
    void setData(const std::vector<vulkan::Vertex>& verts, const std::vector<uint32_t>& inds) {
      vertices = verts;
      indices = inds;
    }

    void clearData() {
      vertices.clear();
      indices.clear();
    }

    void LoadData();

    // Helper info
    size_t getVertexCount() const { return vertices.size(); }
    size_t getIndexCount() const { return indices.size(); }

  private:
  };
}