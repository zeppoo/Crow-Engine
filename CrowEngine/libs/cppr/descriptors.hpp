#pragma once
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
    void AddMember(const char *memberName, size_t size, size_t memberOffset)
    {
      TypeDescriptor member{memberName, size, memberOffset};
      member.type = create_type_info<T>(memberName);
      if (member.type->kind == UNKNOWN) {
        throw std::runtime_error("Unkown Type detected, member name: " + std::string(memberName));
        return;
      }
      members.push_back(std::move(member));
    }

    void *GetMemberMemAdress(void *reference, size_t offset);
    std::string To_Json(const char* filePath, void* structRef);
    void From_Json(const char* filePath, void* structRef);
  };
}
