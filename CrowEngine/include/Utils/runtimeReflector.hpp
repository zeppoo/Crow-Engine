#pragma once
#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"
#include "vulkan/vulkan.h"


namespace reflect
{
  struct TypeDescription
  {
    std::string type;
    const char* name;
    size_t size;

    TypeDescription(std::string type, const char* name, size_t size) : type{type}, name{name}, size{size} {}
    void Serialize(){}
    void SerializeBinary(){}
    void Deserialize(){}
    void DeserializeBinary(){}

  };

  typedef struct TypeDescription reflect_type;

}