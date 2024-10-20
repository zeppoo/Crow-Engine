#pragma once
#include <iostream>
#include <vector>

namespace reflect
{
  void WriteToFile(const char* path, std::string content);
  std::string ReadFromFile(const char* path);
  std::vector<std::string> split(const std::string& str, char delimiter);
  void strip(std::string& str, const char delimiter);
  std::string extract_json_value(const std::string& json, const std::string& field, const std::string& parent);
  std::vector<std::string> extract_json_values(const std::string& json, const std::string& field, const std::string& parent);
}
