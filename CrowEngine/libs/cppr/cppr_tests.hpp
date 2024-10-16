#ifndef CPPR_CPPR_TESTS_HPP
#define CPPR_CPPR_TESTS_HPP

#include "cppr.h"
#include <vector>

namespace cppr_tests
{
  struct Struct1
  {
    int num;
    bool state;
    unsigned long int count;
    std::vector<float> position;
    char* initial;
    std::vector<std::string*> names;
    REFLECT()
  };

  REFLECT_STRUCT_BEGIN(Struct1)
  REFLECT_STRUCT_MEMBER(num)
  REFLECT_STRUCT_MEMBER(state)
  REFLECT_STRUCT_MEMBER(count)
  REFLECT_STRUCT_MEMBER(position)
  REFLECT_STRUCT_MEMBER(initial)
  REFLECT_STRUCT_MEMBER(names)
  REFLECT_STRUCT_END()

  int Test1(const char* filePath);
}

#endif // CPPR_CPPR_TESTS_HPP
