#pragma once
#include <iostream>
#include <cstdint>
#include <variant>

namespace reflect
{
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
  primitive_type get_kind() {
    if constexpr (std::is_pointer_v<T>) {
      return PTR;
    } else if constexpr (std::is_enum_v<T>) {
      return get_kind<typename std::underlying_type<T>::type>();
    } else if constexpr (std::is_void_v<T>) {
      return VOID;
    } else if constexpr (std::is_same_v<T, unsigned char>) {
      return U_CHAR;
    } else if constexpr (std::is_same_v<T, unsigned short>) {
      return U_SHORT;
    } else if constexpr (std::is_same_v<T, unsigned int>) {
      return U_INT;
    } else if constexpr (std::is_same_v<T, unsigned long>) {
      return U_LONG;
    } else if constexpr (std::is_same_v<T, unsigned long long>) {
      return U_LONG_LONG;
    } else if constexpr (std::is_same_v<T, signed char>) {
      return I_CHAR;
    } else if constexpr (std::is_same_v<T, short>) {
      return I_SHORT;
    } else if constexpr (std::is_same_v<T, int>) {
      return I_INT;
    } else if constexpr (std::is_same_v<T, long>) {
      return I_LONG;
    } else if constexpr (std::is_same_v<T, long long>) {
      return I_LONG_LONG;
    } else if constexpr (std::is_same_v<T, float>) {
      return FLOAT;
    } else if constexpr (std::is_same_v<T, double>) {
      return DOUBLE;
    } else if constexpr (std::is_same_v<T, bool>) {
      return BOOL;
    } else if constexpr (std::is_same_v<T, char>) {
      return CHAR;
    } else if constexpr (std::is_same_v<T, std::string>) {
      return STRING;
    } else if constexpr (std::is_integral_v<T>) {
      return I_INT;
    } else {
      throw std::runtime_error("This Type is not supported for reflection!");
      return UNKNOWN;
    }
  }
}
