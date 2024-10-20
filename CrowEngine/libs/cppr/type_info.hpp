#pragma once
#include "serialization.hpp"
#include "type_resolver.hpp"
#include "type_declaration.hpp"
#include <memory>

namespace reflect
{
  struct TypeInfo
  {
    const char* name;
    const primitive_type kind;
    size_t size;

    TypeInfo(const char* name, const primitive_type kind, size_t size);
    virtual ~TypeInfo();

    virtual std::string serialize(void* memberPtr);
    virtual void deserialize(std::string json, std::string parent, void* memberPtr);
  };

  struct PrimitiveTypeInfo : TypeInfo
  {
    PrimitiveTypeInfo(const char* name, const primitive_type kind, size_t size, bool isPtr);

    std::string serialize(void* memberPtr) override;
    void deserialize(std::string content, std::string parent, void* memberPtr) override;
  };

  struct VectorTypeInfo : TypeInfo
  {
    std::unique_ptr<TypeInfo> element_info;

    VectorTypeInfo(const char* name, size_t size, std::unique_ptr<TypeInfo> element_info);

    std::string serialize(void* memberPtr) override;
    void deserialize(std::string content, std::string parent, void* memberPtr) override;
  };

  struct ArrayTypeInfo : TypeInfo
  {
    std::unique_ptr<TypeInfo> element_info;

    ArrayTypeInfo(const char* name, size_t size, std::unique_ptr<TypeInfo> element_info);

    std::string serialize(void* memberPtr) override;
    void deserialize(std::string content, std::string parent, void* memberPtr) override;
  };

  template<typename T>
  struct is_std_vector : std::false_type {};

  template<typename U, typename Alloc>
  struct is_std_vector<std::vector<U, Alloc>> : std::true_type {
    using value_type = U;
  };

  template<typename T>
  std::unique_ptr<TypeInfo> create_type_info(const char* name) {
    if constexpr (std::is_array<T>::value) {
      using element_type = typename std::remove_extent<T>::type;
      return std::make_unique<ArrayTypeInfo>(name, sizeof(T), create_type_info<element_type>("element"));
    }
    else if constexpr (is_std_vector<T>::value) {
      using element_type = typename is_std_vector<T>::value_type;
      return std::make_unique<VectorTypeInfo>(name, sizeof(T), create_type_info<element_type>("element"));
    } else {
      return std::make_unique<PrimitiveTypeInfo>(name, get_kind<T>(), sizeof(T), std::is_pointer<T>::value);
    }
  }

  template <typename T>
  types cast_and_dereference(void* ptr) {
    if (!ptr) throw std::runtime_error("Null pointer");
    T* typedPtr = static_cast<T*>(ptr);
    return *typedPtr;
  }

  types get_type(primitive_type typeKind, void* ptr);
}
