#pragma once
#include "glm/glm.hpp"

namespace engine
{
  struct CameraUBO {
    alignas(16) glm::mat4 view;
    alignas(16) glm::mat4 projection;
    alignas(16) glm::vec3 position;
    alignas(4)  float nearPlane;
    alignas(4)  float farPlane;
    alignas(4)  float fov;
  };

  class Camera {
  public:
  private:
    int UBO_ID;
  };
}