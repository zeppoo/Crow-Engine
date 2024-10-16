#ifndef CPPR_SERIALIZATION_HPP
#define CPPR_SERIALIZATION_HPP

#include <iostream>
#include <vector>

void WriteToFile(const char* path, std::string content);
std::string ReadFromFile(const char* path);
std::vector<std::string> split(const std::string& str, char delimiter);
void strip(std::string& str, const char delimiter);
std::string extract_json_value(const std::string& json, const std::string& field);
std::vector<std::string> extract_json_values(const std::string& json, const std::string& id);

#endif //CPPR_SERIALIZATION_HPP
