#pragma once

#include "crow_lib.hpp"

namespace crowe
{
  void RemoveDuplicatesInList(std::vector<uint32_t> &intList);

  std::vector<char> ReadFile(const std::string &filepath);
}