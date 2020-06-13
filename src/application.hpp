#pragma once
#include "audio.h"
#include <array>
#include <algorithm>
#include <chrono>
#include <fstream>
#include <string>
#include <stdexcept>

// ----------------------------------------------------------------------------
// LOW-LEVEL LIBRARIES
// ----------------------------------------------------------------------------
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// ----------------------------------------------------------------------------
// FRAMEWORK
// ----------------------------------------------------------------------------
#include "mesh.hpp"
#include "program.hpp"
#include "rotate_camera.hpp"
#include "utilities.hpp"
#include "texture.hpp"

// ----------------------------------------------------------------------------
// CONSTANTS
// ----------------------------------------------------------------------------
// Used for clering custom framebuffers(glClearNamedFramebufferfv)
const float clear_color[4] = {0.0, 0.0, 0.0, 1.0};
const float clear_depth[1] = {1.0};

// ----------------------------------------------------------------------------
// UNIFORM STRUCTS
// ----------------------------------------------------------------------------
struct CameraUBO {
  glm::mat4 projection;
  glm::mat4 view;
  glm::vec4 position;
};

struct LightUBO {
  glm::vec4 position;
  glm::vec4 ambient_color;
  glm::vec4 diffuse_color;
  glm::vec4 specular_color;
};

struct ObjectUBO {
  glm::mat4 model_matrix;  // [  0 -  64) bytes
  glm::vec4 ambient_color; // [ 64 -  80) bytes
  glm::vec4 diffuse_color; // [ 80 -  96) bytes

  // Contains shininess in .w element
  glm::vec4 specular_color; // [ 96 - 112) bytes
};

// ----------------------------------------------------------------------------
// APPLICATION
// ----------------------------------------------------------------------------
class Application {

  std::fstream notes;

  std::string current_notes = "------";
  Audio music = Audio("music/Kahoot.mp3");
  const int beat = 250;
  int last_beat = 0;
  std::chrono::high_resolution_clock::time_point begin_time = std::chrono::high_resolution_clock::now();

  const std::array<glm::vec4, 4> kahoot_colors = {{glm::vec4(226, 27, 60, 256) / 256.0f, glm::vec4(18, 104, 205, 256) / 256.0f,
                                                   glm::vec4(216, 158, 0, 256) / 256.0f, glm::vec4(42, 143, 13, 256) / 256.0f}};

  int current_color = 0;

public:
  Application(size_t initial_width, size_t initial_height);
  ~Application();

  void render();

  // ----------------------------------------------------------------------------
  // INPUT EVENTS
  // ----------------------------------------------------------------------------
  void on_resize(GLFWwindow *window, int width, int height);
  void on_mouse_move(GLFWwindow *window, double x, double y);
  void on_mouse_pressed(GLFWwindow *window, int button, int action, int mods);
  void on_key_pressed(GLFWwindow *window, int key, int scancode, int action, int mods);

private:
  GLsizei width;
  GLsizei height;

  // ----------------------------------------------------------------------------
  // CAMERA
  // - By default the camera that rotates around the center is implemented. Feel
  //   free to replace it by your own Camera for ex. FPS one.
  // ----------------------------------------------------------------------------
  RotateCamera camera;
  CameraUBO camera_ubo;
  GLuint camera_buffer = 0;

  // ----------------------------------------------------------------------------
  // PROGRAMS
  // ----------------------------------------------------------------------------
  GLuint draw_lights_program = create_program("shaders/draw_lights.vert", "shaders/draw_lights.frag");
  GLuint draw_object_program = create_program("shaders/draw_object.vert", "shaders/draw_object.frag");
  GLuint draw_object_textured_program = create_program("shaders/draw_object_textured.vert", "shaders/draw_object_textured.frag");
  GLuint postprocess_program = create_program("shaders/postprocess.vert", "shaders/postprocess.frag");

  // ----------------------------------------------------------------------------
  // SCENES/OBJECTS
  // ----------------------------------------------------------------------------
  Mesh cube = Mesh::cube();
  Mesh sphere = Mesh::sphere();
  Mesh teapot = Mesh::teapot();

  // std::vector<std::unique_ptr<Mesh>> obj_test_scene = Mesh::from_file("objects/test_scene.obj");

  // This is the default material that can be used when nothing else is.
  // You might want to create material UBOs and textures inside the Mesh class.

  GLuint default_texture = load_texture_2d("images/default.png");

  // orloj
  std::vector<std::unique_ptr<Mesh>> clock_mesh = Mesh::from_file("objects/clock.obj");
  std::array<ObjectUBO, 7> clock;
  std::array<GLuint, 7> clock_buffer = {0};
  GLuint black_texture = load_texture_2d("images/black.png");
  float direction = 0.1f;
  float y_position = 0.0f;

  // exekutor co berie pracku
  Mesh cube_man_mesh = *Mesh::from_file("objects/cube_man.obj")[0];
  ObjectUBO cube_man_right;
  ObjectUBO cube_man_left;
  GLuint cube_man_right_buffer = 0;
  GLuint cube_man_left_buffer = 0;
  GLuint cube_man_texture = load_texture_2d("objects/Zeleznak.jpg");

  // matematika drsne a svizne
  Mesh mdas_mesh = *Mesh::from_file("objects/mdas.obj")[0];
  ObjectUBO mdas;
  GLuint mdas_buffer = 0;
  GLuint mdas_cover_texture = load_texture_2d("objects/cover.jpg");
  GLuint mdas_pages_texture = load_texture_2d("objects/bookpages.jpg");

  ObjectUBO floor_object;
  GLuint floor_object_buffer = 0;

  // ----------------------------------------------------------------------------
  // LIGHTS
  // ----------------------------------------------------------------------------
  std::vector<LightUBO> lights;
  GLuint lights_buffer = 0;

  // ----------------------------------------------------------------------------
  // POST-PROCESS FRAMEBUFFER
  // ----------------------------------------------------------------------------
  GLuint postprocess_framebuffer = 0;
  GLuint postprocess_framebuffer_color = 0;
  GLuint postprocess_framebuffer_depth = 0;
};