#pragma once
#include "Engine/Scene.hpp"
#include "Engine/Camera.hpp"
#include "Engine/Object.hpp"

namespace renderer
{
  class UBO_Manager {
  private:
    static const int MAX_SCENE_UBO = 5;
    static const int MAX_CAMERA_UBO = 2;
    static const int MAX_OBJECT_UBO = 50;

    engine::SceneUBO scene_ubo[MAX_SCENE_UBO];
    engine::CameraUBO camera_ubo[MAX_CAMERA_UBO];
    engine::ObjectUBO object_ubo[MAX_OBJECT_UBO];

    int scene_uboCount = 0;
    int camera_uboCount = 0;
    int object_uboCount = 0;
  };
}