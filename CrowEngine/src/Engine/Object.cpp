#include "Engine/Object.hpp"
#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

namespace engine
{
 void Object::LoadMesh(const std::string &filepath)
 {

     tinyobj::attrib_t attrib;
     std::vector<tinyobj::shape_t> shapes;
     std::vector<tinyobj::material_t> materials;
   std::string warn, err;
   if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filepath.c_str())) {
     throw std::runtime_error(warn + err);
   }
  mesh.clearData();
   for (const auto &shape : shapes) {
     for (const auto &index : shape.mesh.indices) {
       vulkan::Vertex vertex{};

       if (index.vertex_index >= 0) {
         vertex.position = {
           attrib.vertices[3 * index.vertex_index + 0],
         attrib.vertices[3 * index.vertex_index + 1],
         attrib.vertices[3 * index.vertex_index + 2],
         };
       }

       if (index.vertex_index >= 0) {
         vertex.normal = {
           attrib.normals[3 * index.normal_index + 0],
         attrib.normals[3 * index.normal_index + 1],
         attrib.normals[3 * index.normal_index + 2],
         };
       }

       if (index.texcoord_index >= 0) {
         vertex.uv = {
           attrib.texcoords[3 * index.texcoord_index + 0],
         attrib.texcoords[3 * index.texcoord_index + 1],
         };
       }

       mesh.vertices.push_back(vertex);
     }
   }
 }
}

