#ifndef CPPR_TYPEINFO_HPP
#define CPPR_TYPEINFO_HPP

#include "serialization.hpp"
#include "type_resolver.hpp"
#include "type_declaration.hpp"
#include <memory>

struct TypeInfo
{
  const char* name;
  const primitive_type kind;
  size_t size;

  TypeInfo(const char* name, const primitive_type kind, size_t size);
  virtual ~TypeInfo();

  virtual std::string serialize(void* memberPtr);
  virtual void deserialize(std::string json, void* memberPtr);
};

struct PrimitiveTypeInfo : TypeInfo
{
  PrimitiveTypeInfo(const char* name, const primitive_type kind, size_t size, bool isPtr);

  std::string serialize(void* memberPtr) override;
  void deserialize(std::string content, void* memberPtr) override;
};

struct ArrayTypeInfo : TypeInfo
{
  std::unique_ptr<TypeInfo> element_info;

  ArrayTypeInfo(const char* name, size_t size, std::unique_ptr<TypeInfo> element_info);

  std::string serialize(void* memberPtr) override;
  void deserialize(std::string content, void* memberPtr) override;
};

template<typename T>
std::unique_ptr<TypeInfo> create_type_info(const char* name);

types get_type(primitive_type typeKind, void* ptr);

#endif // CPPR_TYPEINFO_HPP
