#pragma once
#include "descriptors.hpp"

#define REFLECT() \
  static inline void initReflection(reflect::StructDescriptor*); \
  static inline reflect::StructDescriptor structDesc{initReflection};

#define REFLECT_STRUCT_BEGIN(type) \
  inline void type::initReflection(reflect::StructDescriptor* typeDesc) { \
    using T = type; \
    typeDesc->name = #type; \
    typeDesc->size = sizeof(T);

#define REFLECT_STRUCT_MEMBER(name) \
    typeDesc->AddMember<decltype(T::name)>(#name, sizeof(T::name), offsetof(T, name)); \

#define REFLECT_STRUCT_END() \
  }

template <typename Struct>
std::string JsonStructSerializer(const char* filepath, Struct* structsRef) {
   return structsRef->structDesc.To_Json(filepath, structsRef);
}

template <typename Struct, typename... Structs>
std::string JsonStructSerializer(const char* filepath, Struct* firstStruct, Structs*... structs) {
  std::string content;
  content += firstStruct->structDesc.To_Json(filepath, firstStruct);
  if constexpr (sizeof...(structs) > 0) {
    content += ",\n"; // Add comma between struct entries
    content += JsonStructSerializer(filepath, structs...);
  }
  return content;
}

template <typename... Structs>
void SerializeStructsToFile(const char* filepath, Structs*... structs) {
  std::string content = "{\n";
  content += JsonStructSerializer(filepath, structs...);
  content += "\n}";
  reflect::WriteToFile(filepath, content);  // Write to file at the end
}

template <typename Struct>
void DeserializeStructsFromFile(const char* filepath, Struct* Struct_ptr) {
  Struct_ptr->structDesc.From_Json(filepath, Struct_ptr);
}

template <typename Struct, typename... Args>
void DeserializeStructsFromFile(const char* filepath, Struct* first_Struct_ptr, Args*... remaining_Struct_ptrs) {
  first_Struct_ptr->structDesc.From_Json(filepath, first_Struct_ptr);
  DeserializeStructsFromFile(filepath, remaining_Struct_ptrs...);
}

template <typename Struct,typename... Structs>
void SerializeInternalStructsToFile(const char* filepath, Struct* struct_ptr, Structs*... internalStructs) {
  std::string content = "{\n";
  content += struct_ptr->structDesc.To_Json(filepath, struct_ptr);
  content.pop_back();
  content.pop_back();
  content += ",\n";
  content += JsonStructSerializer(filepath, internalStructs...);
  content += "\n}\n}";
  reflect::WriteToFile(filepath, content);  // Write to file at the end
}




