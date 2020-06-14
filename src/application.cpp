#include "application.hpp"
#include <iostream>

Application::Application(size_t initial_width, size_t initial_height) {
  this->width = initial_width;
  this->height = initial_height;

  notes = std::fstream("music/kahoot_notes.txt", std::ios::in);

  if (!notes.is_open()) {
    std::cerr << "pici";
  }

  // --------------------------------------------------------------------------
  // Initialize Data
  // --------------------------------------------------------------------------
  camera_ubo.position = glm::vec4(camera.get_eye_position(), 1.0f);
  camera_ubo.projection = glm::perspective(glm::radians(45.0f), float(width) / float(height), 0.01f, 1000.0f);
  camera_ubo.view = glm::lookAt(camera.get_eye_position(), glm::vec3(6.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

  LightUBO directional_light;
  directional_light.position = glm::vec4(1.0f, 1.0f, 1.0f, 0.0f);
  directional_light.ambient_color = glm::vec4(0.0f);
  directional_light.diffuse_color = glm::vec4(1.0f);
  directional_light.specular_color = glm::vec4(1.0f);
  lights.push_back(directional_light);

  // clock
  // fixing clock
  std::swap(clock_mesh[6], clock_mesh[2]);
  std::swap(clock_mesh[6], clock_mesh[3]);
  std::swap(clock_mesh[6], clock_mesh[4]);
  std::swap(clock_mesh[6], clock_mesh[5]);

  for (size_t i = 0; i < clock.size(); i++) {
    clock[i].model_matrix = glm::mat4(glm::vec4(0.05f, 0.0f, 0.0f, 0.0f), glm::vec4(0.0f, 0.05f, 0.0f, 0.0f),
                                      glm::vec4(0.0f, 0.0f, 0.05f, 0.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
    clock[i].ambient_color = glm::vec4(0.0f);
    clock[i].diffuse_color = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
    clock[i].specular_color = glm::vec4(0.0f, 0.0f, 0.8f, 8.0f);
  }

  cube_man_right.model_matrix = glm::mat4(glm::vec4(0.05f, 0.0f, 0.0f, 0.0f), glm::vec4(0.0f, 0.05f, 0.0f, 0.0f),
                                          glm::vec4(0.0f, 0.0f, 0.05f, 0.0f), glm::vec4(5.0f, 0.0f, 0.0f, 1.0f));
  cube_man_right.ambient_color = glm::vec4(0.0f);
  cube_man_right.diffuse_color = glm::vec4(1.0f);
  cube_man_right.specular_color = glm::vec4(0.0f, 0.0f, 0.8f, 8.0f);

  cube_man_left.model_matrix = glm::mat4(glm::vec4(0.05f, 0.0f, 0.0f, 0.0f), glm::vec4(0.0f, 0.05f, 0.0f, 0.0f),
                                         glm::vec4(0.0f, 0.0f, 0.05f, 0.0f), glm::vec4(-6.0f, 0.0f, 0.0f, 1.0f));
  cube_man_left.model_matrix = glm::rotate(cube_man_left.model_matrix, -3.14f, glm::vec3(0.0f, 1.0f, 0.0f));
  cube_man_left.ambient_color = glm::vec4(0.0f);
  cube_man_left.diffuse_color = glm::vec4(1.0f);
  cube_man_left.specular_color = glm::vec4(0.0f, 0.0f, 0.8f, 8.0f);

  mdas.model_matrix = glm::mat4(glm::vec4(1.0f, 0.0f, 0.0f, 0.0f), glm::vec4(0.0f, 1.0f, 0.0f, 0.0f), glm::vec4(0.0f, 0.0f, 1.0f, 0.0f),
                                glm::vec4(5.0f, 5.0f, 5.0f, 1.0f));
  mdas.ambient_color = glm::vec4(0.0f);
  mdas.diffuse_color = glm::vec4(1.0f);
  mdas.specular_color = glm::vec4(0.0f, 0.0f, 0.8f, 8.0f);

  floor_object.model_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(8.0, 0.01f, 8.0f));
  floor_object.ambient_color = glm::vec4(0.0f);
  floor_object.diffuse_color = glm::vec4(1.0f);
  floor_object.specular_color = glm::vec4(1.0f);

  for (int i = 0; i < max_teapots; i++) {
    ObjectUBO tp;
    tp.model_matrix = glm::mat4(
		glm::vec4(0.0f, 0.0f, 0.0f, 0.0f), 
		glm::vec4(0.0f, 0.0f, 0.0f, 0.0f), 
		glm::vec4(0.0f, 0.0f, 0.0f, 0.0f),
        glm::vec4(100.0f, 10.0f, 0.0f, 1.0f));
    mdas.ambient_color = glm::vec4(0.0f);
    mdas.diffuse_color = glm::vec4(1.0f);
    mdas.specular_color = glm::vec4(0.0f, 0.0f, 0.8f, 8.0f);
    teapot_ubos.emplace_back(tp);
    teapot_times.emplace_back(std::chrono::high_resolution_clock::now());
  }

  // Scatter lights
  for (int x = -10; x < 10; x += 2) {
    for (int y = -10; y < 10; y += 2) {
      glm::vec3 color = (glm::vec3(x, y, 0.0f) + 8.0f) / 16.0f;
      lights.push_back({
          glm::vec4(x, 0.40f, y, 1.0f), // position
          glm::vec4(0.0),               // ambient
          glm::vec4(color, 1.0f),       // diffuse
          glm::vec4(0.0f)               // specular
      });
    }
  }

  // --------------------------------------------------------------------------
  // Create Buffers
  // --------------------------------------------------------------------------
  glCreateBuffers(1, &camera_buffer);
  glNamedBufferStorage(camera_buffer, sizeof(CameraUBO), &camera_ubo, GL_DYNAMIC_STORAGE_BIT);

  glCreateBuffers(1, &lights_buffer);
  glNamedBufferStorage(lights_buffer, lights.size() * sizeof(LightUBO), lights.data(), GL_DYNAMIC_STORAGE_BIT);

  for (size_t i = 0; i < clock.size(); i++) {
    glCreateBuffers(1, &clock_buffer[i]);
    glNamedBufferStorage(clock_buffer[i], sizeof(ObjectUBO), &clock[i], GL_DYNAMIC_STORAGE_BIT);
  }

  glCreateBuffers(1, &cube_man_right_buffer);
  glNamedBufferStorage(cube_man_right_buffer, sizeof(ObjectUBO), &cube_man_right, GL_DYNAMIC_STORAGE_BIT);

  glCreateBuffers(1, &cube_man_left_buffer);
  glNamedBufferStorage(cube_man_left_buffer, sizeof(ObjectUBO), &cube_man_left, GL_DYNAMIC_STORAGE_BIT);

  glCreateBuffers(1, &mdas_buffer);
  glNamedBufferStorage(mdas_buffer, sizeof(ObjectUBO), &mdas, GL_DYNAMIC_STORAGE_BIT);

  glCreateBuffers(1, &teapot_buffer);
  glNamedBufferStorage(teapot_buffer, sizeof(ObjectUBO) * max_teapots, teapot_ubos.data(), GL_DYNAMIC_STORAGE_BIT);

  glCreateBuffers(1, &floor_object_buffer);
  glNamedBufferStorage(floor_object_buffer, sizeof(ObjectUBO), &floor_object, GL_DYNAMIC_STORAGE_BIT);

  // --------------------------------------------------------------------------
  // Create Framebuffer
  // --------------------------------------------------------------------------
  glCreateFramebuffers(1, &postprocess_framebuffer);

  // Initialize color output texture
  glCreateTextures(GL_TEXTURE_2D, 1, &postprocess_framebuffer_color);
  glTextureStorage2D(postprocess_framebuffer_color, 1, GL_RGBA32F, width, height);

  // Initialize depth output texture
  glCreateTextures(GL_TEXTURE_2D, 1, &postprocess_framebuffer_depth);
  glTextureStorage2D(postprocess_framebuffer_depth, 1, GL_DEPTH_COMPONENT32F, width, height);

  // Associate color and depth `attachments` with color and depth `textures`
  glNamedFramebufferTexture(postprocess_framebuffer, GL_COLOR_ATTACHMENT0, postprocess_framebuffer_color, 0);
  glNamedFramebufferTexture(postprocess_framebuffer, GL_DEPTH_ATTACHMENT, postprocess_framebuffer_depth, 0);
}

Application::~Application() {
  glDeleteTextures(1, &postprocess_framebuffer_depth);
  glDeleteTextures(1, &postprocess_framebuffer_color);
  glDeleteTextures(1, &postprocess_framebuffer);

  glDeleteBuffers(1, &lights_buffer);
  glDeleteBuffers(1, &floor_object_buffer);
  glDeleteBuffers(7, clock_buffer.data());
  glDeleteBuffers(1, &cube_man_right_buffer);
  glDeleteBuffers(1, &mdas_buffer);

  glDeleteTextures(1, &black_texture);
  glDeleteTextures(1, &default_texture);

  glDeleteProgram(postprocess_program);
  glDeleteProgram(draw_object_textured_program);
  glDeleteProgram(draw_object_program);
  glDeleteProgram(draw_lights_program);

  glDeleteBuffers(1, &camera_buffer);
}

void Application::render() {

  // --------------------------------------------------------------------------
  // Update data
  // --------------------------------------------------------------------------

  // Camera
  camera_ubo.position = glm::vec4(camera.get_eye_position(), 1.0f);
  camera_ubo.view = glm::lookAt(camera.get_eye_position(), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
  glNamedBufferSubData(camera_buffer, 0, sizeof(CameraUBO), &camera_ubo);

  // --------------------------------------------------------------------------
  // Draw the scene
  // --------------------------------------------------------------------------

  // Bind the Framebuffer
  glBindFramebuffer(GL_FRAMEBUFFER, postprocess_framebuffer);
  auto time_now = std::chrono::high_resolution_clock::now();
  current_color = ((((time_now - begin_time).count() / 1000000) + 700) / 2000) % 4;

  // Clear attachments
  glClearNamedFramebufferfv(postprocess_framebuffer, GL_COLOR, 0, (const float *)&kahoot_colors[current_color].data);
  glClearNamedFramebufferfv(postprocess_framebuffer, GL_DEPTH, 0, clear_depth);

  // Configure fixed function pipeline
  glEnable(GL_DEPTH_TEST);

  glBindBufferBase(GL_UNIFORM_BUFFER, 0, camera_buffer);
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, lights_buffer);

  // Draw floor using the built-in object
  glUseProgram(draw_object_textured_program);
  glBindBufferBase(GL_UNIFORM_BUFFER, 2, floor_object_buffer);
  glBindTextureUnit(0, default_texture);
  cube.draw();


  glUseProgram(draw_object_program);
  y_position += direction;
  if (y_position > 2.0f || y_position < 0.0f) {
    direction = -direction;
  }
  float direction_rotation = 1.0f;
  for (size_t i = 0; i < clock_mesh.size(); i++) {
    direction_rotation = -direction_rotation;
    clock[i].model_matrix = glm::rotate(clock[i].model_matrix, 0.005f + 0.005f * i * direction_rotation, glm::vec3(0.0f, 1.0f, 0.0f));
    clock[i].model_matrix = glm::translate(clock[i].model_matrix, glm::vec3(0.0f, direction * i, 0.0f));
    glNamedBufferSubData(clock_buffer[i], 0, sizeof(ObjectUBO), &clock[i]);
    glBindBufferBase(GL_UNIFORM_BUFFER, 2, clock_buffer[i]);
    clock_mesh[i]->draw();
  }

  glUseProgram(draw_object_textured_program);
  float t = (time_now - begin_time).count() / 1000000000.0f;
  float angle = sinf(t*3.14/2 + 700) * 1.5;

  cube_man_left.model_matrix = glm::rotate(cube_man_left.model_matrix, angle, glm::vec3(0.0f, 1.0f, 0.0f));
  cube_man_right.model_matrix = glm::rotate(cube_man_right.model_matrix, -angle, glm::vec3(0.0f, 1.0f, 0.0f));
  
  glNamedBufferSubData(cube_man_right_buffer, 0, sizeof(ObjectUBO), &cube_man_right);
  glNamedBufferSubData(cube_man_left_buffer, 0, sizeof(ObjectUBO), &cube_man_left);

  glBindBufferBase(GL_UNIFORM_BUFFER, 2, cube_man_right_buffer);
  glBindTextureUnit(0, cube_man_texture);
  cube_man_mesh.draw();

  glBindBufferBase(GL_UNIFORM_BUFFER, 2, cube_man_left_buffer);
  glBindTextureUnit(0, cube_man_texture);
  cube_man_mesh.draw();

  cube_man_left.model_matrix = glm::rotate(cube_man_left.model_matrix, -angle, glm::vec3(0.0f, 1.0f, 0.0f));
  cube_man_right.model_matrix = glm::rotate(cube_man_right.model_matrix, +angle, glm::vec3(0.0f, 1.0f, 0.0f));

  if ((time_now - begin_time).count() / 1000000 >= last_beat + beat) {
    last_beat = last_beat + beat;
    if (!std::getline(notes, current_notes)) {
      std::cout << "end";
      current_notes = "123456";
    }

    for (size_t i = 0; i < current_notes.size(); i++) {
      int dif = 1;
      int n = current_notes.at(i) - '0';
      if (i == 0 || i == 2) {
        dif == 0;
      }
      if (n < 12 && n >= 0) {
        int move = ((n - 5 * i - dif) * 2) + 20;
        ObjectUBO tp;
        tp.model_matrix = glm::mat4(
			glm::vec4(0.5f, 0.0f, 0.0f, 0.0f), 
			glm::vec4(0.0f, 0.5f, 0.0f, 0.0f), 
			glm::vec4(0.0f, 0.0f, 0.5f, 0.0f),
            glm::vec4(move, default_height, 3.0f, 1.0f));
        mdas.ambient_color = glm::vec4(0.0f);
        mdas.diffuse_color = glm::vec4(1.0f);
        mdas.specular_color = glm::vec4(0.0f, 0.0f, 0.8f, 8.0f);
        teapot_ubos[last_teapot_index] = tp;
        teapot_times[last_teapot_index] = std::chrono::high_resolution_clock::now();
        last_teapot_index = (last_teapot_index + 1) % max_teapots;
      }
    }
  }

  glUseProgram(draw_teapots_program);
  for (size_t i = 0; i < max_teapots; i++) {
    float t = (time_now - teapot_times[i]).count() / 1000000000.0f;
    float new_y = default_height - ((t * t) * 9.8f) / 2.0f;
    teapot_ubos[i].model_matrix[3][1] = new_y;
  }
  glNamedBufferSubData(teapot_buffer, 0, sizeof(ObjectUBO) * max_teapots, teapot_ubos.data());
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, teapot_buffer);
  glBindVertexArray(teapot.get_vao());
  glDrawElementsInstanced(teapot.get_mode(), teapot.get_indices_count(), GL_UNSIGNED_INT, nullptr, teapot_ubos.size());
  
  // Draw lights using Instanced rendering
  glUseProgram(draw_lights_program);

  glBindBufferBase(GL_UNIFORM_BUFFER, 0, camera_buffer);
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, lights_buffer);


  // --------------------------------------------------------------------------
  // Apply post-process
  // --------------------------------------------------------------------------

  // Bind back the default framebuffer (0)
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  // Clear color buffer
  glClear(GL_COLOR_BUFFER_BIT);

  // Set fixed function pipeline
  glDisable(GL_DEPTH_TEST); // Disable depth test - we do not need it
  glViewport(0, 0, this->width, this->height);

  // Use post-process program
  glUseProgram(postprocess_program);

  // Bind the output from previous program as input texture to the post-process program
  glBindTextureUnit(0, postprocess_framebuffer_color);

  // Draw the full-screen triangle
  glDrawArrays(GL_TRIANGLES, 0, 3);
}

void Application::on_resize(GLFWwindow *window, int width, int height) {
  this->width = width;
  this->height = height;

  glDeleteTextures(1, &postprocess_framebuffer_color);
  glDeleteTextures(1, &postprocess_framebuffer_depth);

  // Re-Initialize color output texture
  glCreateTextures(GL_TEXTURE_2D, 1, &postprocess_framebuffer_color);
  glTextureStorage2D(postprocess_framebuffer_color, 1, GL_RGBA32F, width, height);

  // Re-Initialize depth output texture
  glCreateTextures(GL_TEXTURE_2D, 1, &postprocess_framebuffer_depth);
  glTextureStorage2D(postprocess_framebuffer_depth, 1, GL_DEPTH_COMPONENT32F, width, height);

  // Re-Associate color and depth `attachments` with color and depth `textures`
  glNamedFramebufferTexture(postprocess_framebuffer, GL_COLOR_ATTACHMENT0, postprocess_framebuffer_color, 0);
  glNamedFramebufferTexture(postprocess_framebuffer, GL_DEPTH_ATTACHMENT, postprocess_framebuffer_depth, 0);
}
void Application::on_mouse_move(GLFWwindow *window, double x, double y) { camera.on_mouse_move(x, y); }
void Application::on_mouse_pressed(GLFWwindow *window, int button, int action, int mods) { camera.on_mouse_button(button, action, mods); }
void Application::on_key_pressed(GLFWwindow *window, int key, int scancode, int action, int mods) {}