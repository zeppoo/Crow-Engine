#ifndef CPPR_TYPE_RESOLVER_HPP
#define CPPR_TYPE_RESOLVER_HPP

#include "type_declaration.hpp"
#include <sstream>
#include <vector>

std::string toString(void* value);
std::string toString(bool value);
std::string toString(char value);
std::string toString(unsigned char value);
std::string toString(unsigned short value);
std::string toString(unsigned int value);
std::string toString(unsigned long value);
std::string toString(unsigned long long value);
std::string toString(signed char value);
std::string toString(short value);
std::string toString(int value);
std::string toString(long value);
std::string toString(long long value);
std::string toString(float value);
std::string toString(double value);
std::string toString(const std::string& value);
std::string toString(types value);

template <typename T>
T fromString(const std::string& value);

template <typename T>
T* toTypePtr(void* ptr);

template <typename T>
std::vector<T>& toTypeVector(void* ptr);


#endif //CPPR_TYPE_RESOLVER_HPP
