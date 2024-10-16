#ifndef CPPR_TYPE_DECLERATION_HPP
#define CPPR_TYPE_DECLERATION_HPP

#include <iostream>
#include <cstdint>
#include <variant>

enum primitive_type : uint8_t
{
  UNKNOWN = 0,
  PTR,
  VOID,
  BOOL,
  CHAR,
  U_CHAR,
  U_SHORT,
  U_INT,
  U_LONG,
  U_LONG_LONG,
  I_CHAR,
  I_SHORT,
  I_INT,
  I_LONG,
  I_LONG_LONG,
  FLOAT,
  DOUBLE,
  STRING,
  ARRAY,
  ENUM
};

using types = std::variant<
    void*,
    bool,
    char,
    unsigned char,
    unsigned short,
    unsigned int,
    unsigned long,
    unsigned long long,
    signed char,
    short,
    int,
    long,
    long long,
    float,
    double,
    std::string
>;

template <typename T>
primitive_type get_kind();

#endif // CPPR_TYPE_DECLERATION_HPP
