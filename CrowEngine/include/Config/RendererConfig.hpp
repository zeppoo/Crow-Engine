#pragma once
#include "crow_lib.hpp"
#include "cppr.hpp"

namespace configs
{
  struct RendererConfig
  {
    REFLECT()
    std::string nameID;
    std::string shadersFilePath = "path/to/file";

    bool enableMSAA = false;
    bool enableLight = false;
    bool enableShadows = false;
  };

  REFLECT_STRUCT_BEGIN(RendererConfig)
    REFLECT_STRUCT_MEMBER(nameID)
    REFLECT_STRUCT_MEMBER(shadersFilePath)
    REFLECT_STRUCT_MEMBER(enableMSAA)
    REFLECT_STRUCT_MEMBER(enableLight)
    REFLECT_STRUCT_MEMBER(enableShadows)
  REFLECT_STRUCT_END()
}