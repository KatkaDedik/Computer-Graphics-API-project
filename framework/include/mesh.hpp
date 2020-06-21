#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <vector>
#include <string>
#include <memory>

class Mesh {
public:
  Mesh(std::vector<float> vertices, std::vector<uint32_t> indices, GLenum mode = GL_TRIANGLES, GLint position_location = 0,
       GLint normal_location = 1, GLint tex_coord_location = 2)
      : Mesh(vertices, std::vector<float>(), std::vector<float>(), indices, mode, position_location, normal_location, tex_coord_location) {}

  Mesh(std::vector<float> vertices, std::vector<float> normals, std::vector<uint32_t> indices, GLenum mode = GL_TRIANGLES,
       GLint position_location = 0, GLint normal_location = 1, GLint tex_coord_location = 2)
      : Mesh(vertices, normals, std::vector<float>(), indices, mode, position_location, normal_location, tex_coord_location) {}

  Mesh(std::vector<float> vertices, std::vector<float> normals, std::vector<float> tex_coords, std::vector<uint32_t> indices,
       GLenum mode = GL_TRIANGLES, GLint position_location = 0, GLint normal_location = 1, GLint tex_coord_location = 2);

  Mesh(const Mesh &other);

  void recreate_vao(GLint position_location = 0, GLint normal_location = 1, GLint tex_coord_location = 2, GLint tangent_location = 3,
                    GLint bitangent_location = 4);
  void draw();

  static Mesh from_interleaved(std::vector<float> interleaved_vertices, std::vector<uint32_t> indices, GLenum mode = GL_TRIANGLES,
                               GLint position_location = 0, GLint normal_location = 1, GLint tex_coord_location = 2);

  static std::vector<std::unique_ptr<Mesh>> from_file(const std::string &file_name, GLint position_location = 0, GLint normal_location = 1,
                                                      GLint tex_coord_location = 2, GLint tangent_location = 3,
                                                      GLint bitangent_location = 4);

  static Mesh cube(GLint position_location = 0, GLint normal_location = 1, GLint tex_coord_location = 2);
  static Mesh sphere(GLint position_location = 0, GLint normal_location = 1, GLint tex_coord_location = 2);
  static Mesh teapot(GLint position_location = 0, GLint normal_location = 1, GLint tex_coord_location = 2);

  GLuint get_vao() { return this->vao; }
  GLenum get_mode() { return this->mode; }

  size_t get_vertices_count() { return this->vertices_count; }
  size_t get_indices_count() { return this->indices_count; }

  GLuint get_vertices_buffer() { return this->vertices_buffer; }
  GLuint get_normals_buffer() { return this->normals_buffer; }
  GLuint get_tex_coords_buffer() { return this->tex_coords_buffer; }
  GLuint get_indices_buffer() { return this->indices_buffer; }

  ~Mesh();

  void computeTangentBasis(
      // inputs
      std::vector<float> &vertices, std::vector<float> &uvs,
      // outputs
      std::vector<glm::vec3> &tangents, std::vector<glm::vec3> &bitangents);

private:
  GLuint vao = 0;

  size_t vertices_count = 0;
  GLuint vertices_buffer = 0;
  GLuint normals_buffer = 0;
  GLuint tex_coords_buffer = 0;
  GLuint tangent_buffer = 0;
  GLuint bitangent_buffer = 0;

  size_t indices_count = 0;
  GLuint indices_buffer = 0;

  GLint position_location = -1;
  GLint normal_location = -1;
  GLint tex_coord_location = -1;
  GLint bitangent_location = -1;
  GLint tangent_location = -1;

  GLenum mode = GL_TRIANGLES;
};
