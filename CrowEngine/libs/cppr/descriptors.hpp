#ifndef CPPR_DESCRIPTORS_HPP
#define CPPR_DESCRIPTORS_HPP

#include "type_info.hpp"

namespace reflect
{
  struct TypeDescriptor
  {
    size_t offset;
    std::unique_ptr<TypeInfo> type;

    TypeDescriptor(const char *memberName, size_t size, size_t memberOffset);
  };

  struct StructDescriptor
  {
    const char *name;
    size_t size;
    std::vector<TypeDescriptor> members;

    StructDescriptor(const char* name, size_t size);
    StructDescriptor(void (*init)(StructDescriptor*));

    template<typename T>
    void AddMember(const char *memberName, size_t size, size_t memberOffset);

    void *GetMemberMemAdress(void *reference, size_t offset);
    void To_Json(const char* filePath, void* structRef);
    void From_Json(const char* filePath, void* structRef);
  };
}

#endif // CPPR_DESCRIPTORS_HPP
