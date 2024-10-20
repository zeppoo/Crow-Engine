#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <unordered_set>
#include <fstream>

namespace crowe
{
  template<typename T>
  void RemoveDuplicatesInList(std::vector<T> &list)
  {
    std::unordered_set<T> seen;
    size_t index = 0;

    for (const T &num: list) {
      if (seen.insert(num).second) {  // insert() returns a pair, .second is true if the element was inserted
        list[index++] = num;
      }
    }

    list.resize(index);  // Resize the list to remove extra elements
  }

  std::vector<char> ReadFile(const std::string &filepath)
  {
    std::ifstream file{filepath, std::ios::ate | std::ios::binary};
    if (!file.is_open()) {
      throw std::runtime_error("failed to open file: " + filepath);
    }
    size_t fileSize = static_cast<size_t>(file.tellg());
    std::vector<char> buffer(fileSize);
    file.seekg(0);
    file.read(buffer.data(), fileSize);
    file.close();
    return buffer;
  }
}