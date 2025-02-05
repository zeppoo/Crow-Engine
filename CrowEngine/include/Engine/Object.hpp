#pragma once
#include "glm/glm.hpp"
#include "Renderer/Mesh.hpp"

namespace engine
{
struct ObjectUBO {
  alignas(16) glm::mat4 model;
  alignas(16) glm::vec4 color;
};

class Object {
public:
  glm::vec3 position;
  glm::vec3 rotation;
  glm::vec3 scale;

  renderer::Mesh mesh;

  void LoadMesh(const std::string &filepath);
  
private:
  int UBO_ID;
};
}