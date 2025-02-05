#pragma once
#include "glm/glm.hpp"

#include "Object.hpp"

namespace engine {

struct SceneUBO {
  alignas(16) glm::vec4 ambientLight;
  alignas(16) glm::vec4 directionalLight;
  alignas(8)  glm::vec2 screenSize;
  alignas(4)  float time;
  alignas(4)  float deltaTime;
};

class Scene {
public:
  std::vector<Object> SceneObjects;

private:
  int UBO_ID;
};

}
