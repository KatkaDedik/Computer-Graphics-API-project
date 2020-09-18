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
  camera_ubo.view = glm::lookAt(camera.get_eye_position(), camera.get_direction(), glm::vec3(0.0f, 1.0f, 0.0f));

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
    clock[i].model_matrix = glm::mat4(glm::vec4(0.07f, 0.0f, 0.0f, 0.0f), glm::vec4(0.0f, 0.07f, 0.0f, 0.0f),
                                      glm::vec4(0.0f, 0.0f, 0.07f, 0.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
    clock[i].ambient_color = glm::vec4(0.0f);
    clock[i].diffuse_color = glm::vec4(0.1f, 0.1f, 0.1f, 1.0f);
    clock[i].specular_color = glm::vec4(0.8f, 0.8f, 0.8f, 50.0f);
  }

  cube_man_right.model_matrix = glm::mat4(
	  glm::vec4(0.05f, 0.0f, 0.0f, 0.0f),
	  glm::vec4(0.0f, 0.05f, 0.0f, 0.0f),
      glm::vec4(0.0f, 0.0f, 0.05f, 0.0f), 
	  glm::vec4(15.0f, 0.0f, 0.0f, 1.0f));
  cube_man_right.ambient_color = glm::vec4(0.0f);
  cube_man_right.diffuse_color = glm::vec4(1.0f);
  cube_man_right.specular_color = glm::vec4(0.0f, 0.0f, 0.8f, 8.0f);

  cube_man_left.model_matrix = glm::mat4(
	  glm::vec4(0.05f, 0.0f, 0.0f, 0.0f), 
	  glm::vec4(0.0f, 0.05f, 0.0f, 0.0f),
      glm::vec4(0.0f, 0.0f, 0.05f, 0.0f), 
	  glm::vec4(-15.0f, 0.0f, 0.0f, 1.0f));
  cube_man_left.model_matrix = glm::rotate(cube_man_left.model_matrix, -3.14f, glm::vec3(0.0f, 1.0f, 0.0f));
  cube_man_left.ambient_color = glm::vec4(0.0f);
  cube_man_left.diffuse_color = glm::vec4(1.0f);
  cube_man_left.specular_color = glm::vec4(0.0f, 0.0f, 0.8f, 8.0f);

  mdas.model_matrix = glm::rotate(glm::mat4(
      glm::vec4(0.1f, 0.0f, 0.0f, 0.0f), 
      glm::vec4(0.0f, 0.1f, 0.0f, 0.0f), 
      glm::vec4(0.0f, 0.0f, 0.1f, 0.0f),
      glm::vec4(0.0f, 10.0f, 0.0f, 1.0f)),
      3.14f/2.0f, glm::vec3(1.0f,0.0f,0.0f));
  mdas.ambient_color = glm::vec4(0.0f);
  mdas.diffuse_color = glm::vec4(1.0f);
  mdas.specular_color = glm::vec4(0.0f, 0.0f, 0.2f, 8.0f);

  floor_object.model_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(20.0, 0.01f, 20.0f));
  floor_object.ambient_color = glm::vec4(0.0f);
  floor_object.diffuse_color = glm::vec4(1.0f);
  floor_object.specular_color = glm::vec4(0.2f, 0.2f, 0.2f, 8.0f);

  beer.model_matrix = glm::mat4(
	  glm::vec4(0.3f, 0.0f, 0.0f, 0.0f), 
	  glm::vec4(0.0f, 0.3f, 0.0f, 0.0f), 
	  glm::vec4(0.0f, 0.0f, 0.3f, 0.0f),
      glm::vec4(0.0f, 10.0f, 0.0f, 1.0f));
  beer.ambient_color = glm::vec4(0.0f);
  beer.diffuse_color = glm::vec4(1.0f);
  beer.specular_color = glm::vec4(0.0f, 0.0f, 0.2f, 8.0f);

  wall.model_matrix = glm::mat4(
      glm::vec4(5.3f, 0.0f, 0.0f, 0.0f), 
      glm::vec4(0.0f, 5.3f, 0.0f, 0.0f), 
      glm::vec4(0.0f, 0.0f, 5.3f, 0.0f),
      glm::vec4(-1.5f, 0.0f, -20.0f, 1.0f));
  wall.ambient_color = glm::vec4(0.0f);
  wall.diffuse_color = glm::vec4(1.0f);
  wall.specular_color = glm::vec4(0.0f, 0.0f, 0.2f, 8.0f);

  electro.model_matrix = glm::mat4(
      glm::vec4(0.05f, 0.0f, 0.0f, 0.0f), 
      glm::vec4(0.0f, 0.05f, 0.0f, 0.0f), 
      glm::vec4(0.0f, 0.0f, 0.05f, 0.0f),
      glm::vec4(-10.0f, 3.0f, -10.0f, 1.0f));
  electro.ambient_color = glm::vec4(0.0f);
  electro.diffuse_color = glm::vec4(1.0f);
  electro.specular_color = glm::vec4(0.0f, 0.0f, 0.2f, 8.0f);

  car.model_matrix = glm::rotate(glm::mat4(
      glm::vec4(0.07f, 0.0f, 0.0f, 0.0f),
      glm::vec4(0.0f, 0.07f, 0.0f, 0.0f),
      glm::vec4(0.0f, 0.0f, 0.07f, 0.0f),
      glm::vec4(10.0f, 2.0f, -10.0f, 1.0f)),
      45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
  car.ambient_color = glm::vec4(0.0f);
  car.diffuse_color = glm::vec4(1.0f);
  car.specular_color = glm::vec4(0.0f, 0.0f, 0.2f, 8.0f);

  for (int i = 0; i < max_teapots; i++) {
    ObjectUBO tp;
    tp.model_matrix = glm::mat4(
		glm::vec4(0.0f, 0.0f, 0.0f, 0.0f), 
		glm::vec4(0.0f, 0.0f, 0.0f, 0.0f), 
		glm::vec4(0.0f, 0.0f, 0.0f, 0.0f),
        glm::vec4(100.0f, 10.0f, 0.0f, 1.0f));
    tp.ambient_color = glm::vec4(0.0f);
    tp.diffuse_color = glm::vec4(1.0f);
    tp.specular_color = glm::vec4(0.0f, 0.0f, 0.8f, 8.0f);
    teapot_ubos.emplace_back(tp);
    teapot_times.emplace_back(std::chrono::high_resolution_clock::now());

   
  }
  
  // Scatter lights
  for (int y = 0; y < 10; y++) {
    for (int x = 0; x < 5; x++) {
      lights.push_back({
          glm::vec4(x + 5 + y, 0.30f, -y - 5, 1.0f), // position
          glm::vec4(0.0),               // ambient
          glm::vec4(0.0f, 0.0f, 0.7f, 1.0f),       // diffuse
          glm::vec4(0.0f)               // specular
      });
    }
  }

  fog.color = glm::vec4(0.0f, 0.0f, 0.6f, 8.0f);
  fog.density = 0.02f;
  fog.start = 0.05f;
  fog.end = 1.0f;
  fog.scale = 1.5f;


  // --------------------------------------------------------------------------
  // Create Buffers
  // --------------------------------------------------------------------------
  glCreateBuffers(1, &camera_buffer);
  glNamedBufferStorage(camera_buffer, sizeof(CameraUBO), &camera_ubo, GL_DYNAMIC_STORAGE_BIT);

  glCreateBuffers(1, &lights_buffer);
  glNamedBufferStorage(lights_buffer, lights.size() * sizeof(LightUBO), lights.data(), GL_DYNAMIC_STORAGE_BIT);

  glCreateBuffers(1, &fog_buffer);
  glNamedBufferStorage(fog_buffer, sizeof(FogUBO), &fog, GL_DYNAMIC_STORAGE_BIT);

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

  glCreateBuffers(1, &beer_buffer);
  glNamedBufferStorage(beer_buffer, sizeof(ObjectUBO), &beer, GL_DYNAMIC_STORAGE_BIT);

  glCreateBuffers(1, &wall_buffer);
  glNamedBufferStorage(wall_buffer, sizeof(ObjectUBO), &wall, GL_DYNAMIC_STORAGE_BIT);

  glCreateBuffers(1, &car_buffer);
  glNamedBufferStorage(car_buffer, sizeof(ObjectUBO), &car, GL_DYNAMIC_STORAGE_BIT);

  glCreateBuffers(1, &electro_buffer);
  glNamedBufferStorage(electro_buffer, sizeof(ObjectUBO), &electro, GL_DYNAMIC_STORAGE_BIT);

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


  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);

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
  glDeleteBuffers(1, &electro_buffer);
  glDeleteBuffers(1, &car_buffer);
  glDeleteBuffers(1, &wall_buffer);
  glDeleteBuffers(1, &beer_buffer);
  glDeleteBuffers(1, &fog_buffer);

  glDeleteTextures(1, &black_texture);
  glDeleteTextures(1, &default_texture);

  glDeleteProgram(postprocess_program);
  glDeleteProgram(draw_object_textured_program);
  glDeleteProgram(draw_object_program);
  glDeleteProgram(draw_lights_program);
  glDeleteProgram(draw_object_normal_textured_program);

  glDeleteBuffers(1, &camera_buffer);
}

void Application::render() {

    time_now = std::chrono::high_resolution_clock::now();

	if (!is_playing) {
        begin_time = std::chrono::high_resolution_clock::now();
		music = std::make_unique<Audio>("music/Kahoot.mp3");
		is_playing = true;
	}
	

  // --------------------------------------------------------------------------
  // Update data
  // --------------------------------------------------------------------------

  // Camera
  camera_ubo.position = glm::vec4(camera.get_eye_position(), 1.0f);
  camera_ubo.view = glm::lookAt(camera.get_eye_position(), camera.get_direction(), glm::vec3(0.0f, 1.0f, 0.0f));
  glNamedBufferSubData(camera_buffer, 0, sizeof(CameraUBO), &camera_ubo);

  // --------------------------------------------------------------------------
  // Draw the scene
  // --------------------------------------------------------------------------

  current_color = ((((time_now - begin_time).count() / 1000000) + 700) / 2000) % 4;
  fog.color = kahoot_colors[current_color];
  glNamedBufferSubData(fog_buffer, 0, sizeof(FogUBO), &fog);

  // Clear attachments
  glClearColor(kahoot_colors[current_color].r, kahoot_colors[current_color].g, kahoot_colors[current_color].b, 1.0f);
  glClearDepth(static_cast<double>(clear_depth[0]));
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  
  // Configure fixed function pipeline
  glEnable(GL_DEPTH_TEST);

  glBindBufferBase(GL_UNIFORM_BUFFER, 0, camera_buffer);
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, lights_buffer);

  // Draw 
  glUseProgram(draw_object_normal_textured_program);
  glBindBufferBase(GL_UNIFORM_BUFFER, 2, floor_object_buffer);
  glBindBufferBase(GL_UNIFORM_BUFFER, 4, fog_buffer);
  glBindTextureUnit(0, floor_texture);
  glBindTextureUnit(1, floor_normal_map);
  cube.draw();
  
  draw_mdas();
  draw_clock();
  draw_executor();
  draw_teapots();
  draw_car();


  glUseProgram(draw_object_normal_textured_program);
  glBindBufferBase(GL_UNIFORM_BUFFER, 2, beer_buffer);
  glBindBufferBase(GL_UNIFORM_BUFFER, 4, fog_buffer);
  glBindTextureUnit(0, beer_texture);

  float t = (time_now - begin_time).count() / 1000000000.0f;
  float angle = (t * 3.14 / 3) ;

  beer.model_matrix = glm::translate(beer.model_matrix, glm::vec3(sinf(angle) * 25, cosf(angle) * 5, -cosf(angle) * 25));
  glNamedBufferSubData(beer_buffer, 0, sizeof(ObjectUBO), &beer);

  beer_mesh.draw();

  beer.model_matrix = glm::translate(beer.model_matrix, glm::vec3(-sinf(angle) * 25, -cosf(angle) * 5, cosf(angle) * 25));


  beer_mesh.draw();

  glBindBufferBase(GL_UNIFORM_BUFFER, 2, wall_buffer);
  glBindBufferBase(GL_UNIFORM_BUFFER, 4, fog_buffer);
  glBindTextureUnit(0, wall_texture);
  glBindTextureUnit(1, wall_normal_texture);
  wall_mesh.draw();

  glBindBufferBase(GL_UNIFORM_BUFFER, 2, electro_buffer);
  glBindBufferBase(GL_UNIFORM_BUFFER, 4, fog_buffer);
  glBindTextureUnit(0, electro_texture);
  glBindTextureUnit(1, electro_normal_texture);
  electro_mesh.draw();

  glUseProgram(draw_teapots_program);
  for (size_t i = 0; i < max_teapots; i++) {
    float t = (time_now - teapot_times[i]).count() / 1000000000.0f;
    float new_y = default_height - ((t * t) * 9.8f) / 2.0f;
    teapot_ubos[i].model_matrix[3][1] = new_y;
    teapot_ubos[i].diffuse_color[1] = - new_y / 15;
  }
  glNamedBufferSubData(teapot_buffer, 0, sizeof(ObjectUBO) * max_teapots, teapot_ubos.data());
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, teapot_buffer);
  glBindVertexArray(teapot.get_vao());
  glDrawElementsInstanced(teapot.get_mode(), teapot.get_indices_count(), GL_UNSIGNED_INT, nullptr, teapot_ubos.size());
  
  // Draw lights using Instanced rendering
  glUseProgram(draw_lights_program);

  glBindBufferBase(GL_UNIFORM_BUFFER, 0, camera_buffer);
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, lights_buffer);
}


void Application::draw_clock() {
  glBindBufferBase(GL_UNIFORM_BUFFER, 4, fog_buffer);
  glBindTextureUnit(0, beer_texture);
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
}

void Application::draw_car() {
  glUseProgram(draw_object_program);
  glBindBufferBase(GL_UNIFORM_BUFFER, 4, fog_buffer);
  float t = (time_now - begin_time).count() / 1000000000.0f;
  float angle = cosf(t * 3.14 / 2 + 700) * 0.2;
  float x = sinf(angle) * 5.0f;
  float move = 40.0f;
  float delay = 0.02;

  if (angle < 0) {

      angle += delay;

      if (angle < 0) {
          car.model_matrix = glm::translate(car.model_matrix, glm::vec3( move, 0.0f, 0.0f));
          car.model_matrix = glm::rotate(car.model_matrix, angle, glm::vec3(0.0f, 0.0f, 1.0f));
          car.model_matrix = glm::translate(car.model_matrix, glm::vec3(-move + x, 0, 0.0f));

          glNamedBufferSubData(car_buffer, 0, sizeof(ObjectUBO), &car);
          glBindBufferBase(GL_UNIFORM_BUFFER, 2, car_buffer);
          for (const auto &mesh : car_mesh) {
            mesh->draw();
          }
          car.model_matrix = glm::translate(car.model_matrix, glm::vec3(move - x, 0, 0.0f));
          car.model_matrix = glm::rotate(car.model_matrix, -angle, glm::vec3(0.0f, 0.0f, 1.0f));
          car.model_matrix = glm::translate(car.model_matrix, glm::vec3(-move, 0.0f, 0.0f));
      
      } else {
        glNamedBufferSubData(car_buffer, 0, sizeof(ObjectUBO), &car);
        glBindBufferBase(GL_UNIFORM_BUFFER, 2, car_buffer);
        for (const auto &mesh : car_mesh) {
          mesh->draw();
        }
      }

  } else {

    angle -= delay;

    if (angle > 0) {
    
      car.model_matrix = glm::translate(car.model_matrix, glm::vec3(-move, 0.0f, 0.0f));
      car.model_matrix = glm::rotate(car.model_matrix, angle, glm::vec3(0.0f, 0.0f, 1.0f));
      car.model_matrix = glm::translate(car.model_matrix, glm::vec3(move + x, 0, 0.0f));

      glNamedBufferSubData(car_buffer, 0, sizeof(ObjectUBO), &car);
      glBindBufferBase(GL_UNIFORM_BUFFER, 2, car_buffer);
      for (const auto &mesh : car_mesh) {
        mesh->draw();
      }
      car.model_matrix = glm::translate(car.model_matrix, glm::vec3(-move - x, 0, 0.0f));
      car.model_matrix = glm::rotate(car.model_matrix, -angle, glm::vec3(0.0f, 0.0f, 1.0f));
      car.model_matrix = glm::translate(car.model_matrix, glm::vec3(move, 0.0f, 0.0f));

    } else {
      glNamedBufferSubData(car_buffer, 0, sizeof(ObjectUBO), &car);
      glBindBufferBase(GL_UNIFORM_BUFFER, 2, car_buffer);
      for (const auto &mesh : car_mesh) {
        mesh->draw();
      }
    }
  }}

void Application::draw_executor() {
  glUseProgram(draw_object_normal_textured_program);
  glBindBufferBase(GL_UNIFORM_BUFFER, 4, fog_buffer);
  float t = (time_now - begin_time).count() / 1000000000.0f;
  float angle = sinf(t * 3.14 / 2 + 700) * 1.5;

  cube_man_left.model_matrix = glm::rotate(cube_man_left.model_matrix, angle, glm::vec3(0.0f, 1.0f, 0.0f));
  cube_man_right.model_matrix = glm::rotate(cube_man_right.model_matrix, -angle, glm::vec3(0.0f, 1.0f, 0.0f));

  glNamedBufferSubData(cube_man_right_buffer, 0, sizeof(ObjectUBO), &cube_man_right);
  glNamedBufferSubData(cube_man_left_buffer, 0, sizeof(ObjectUBO), &cube_man_left);

  glBindBufferBase(GL_UNIFORM_BUFFER, 2, cube_man_right_buffer);
  glBindTextureUnit(0, cube_man_texture);
  glBindTextureUnit(1, cube_man_normal_map);
  cube_man_mesh.draw();

  glBindBufferBase(GL_UNIFORM_BUFFER, 2, cube_man_left_buffer);
  cube_man_mesh.draw();

  cube_man_left.model_matrix = glm::rotate(cube_man_left.model_matrix, -angle, glm::vec3(0.0f, 1.0f, 0.0f));
  cube_man_right.model_matrix = glm::rotate(cube_man_right.model_matrix, +angle, glm::vec3(0.0f, 1.0f, 0.0f));
}

void Application::draw_mdas() { 
  glUseProgram(draw_object_textured_program);
  glBindBufferBase(GL_UNIFORM_BUFFER, 2, mdas_buffer);
  glBindTextureUnit(0, mdas_texture);

  float t = (time_now - begin_time).count() / 1000000000.0f;
  float angle = (t * 3.14 / 2 + 700) * 1.5;

  mdas.model_matrix = glm::translate(mdas.model_matrix, glm::vec3(sinf(angle) * 35, cosf(angle) * 40, cosf(angle) * 10));
  glNamedBufferSubData(mdas_buffer, 0, sizeof(ObjectUBO), &mdas);

  mdas_mesh.draw();

  mdas.model_matrix = glm::translate(mdas.model_matrix, glm::vec3(-sinf(angle) * 35, -cosf(angle) * 40, -cosf(angle) * 10));
}

void Application::draw_teapots(){
  if ((time_now - begin_time).count() / 1000000 >= last_beat + beat) {
    last_beat = last_beat + beat;
    if (!std::getline(notes, current_notes)) {
      std::cout << "end";
      current_notes = "GIKMOQSU";
    }
    int z = -14;
    for (size_t i = 0; i < current_notes.size(); i++) {
      int n = current_notes.at(i);

      if (n != ';') {
        n -= '=';
        int move = n * 0.7f + z * (-4) - 75;
        ObjectUBO tp;
        tp.model_matrix = glm::mat4(
            glm::vec4(0.5f, 0.0f, 0.0f, 0.0f),
            glm::vec4(0.0f, 0.5f, 0.0f, 0.0f), 
            glm::vec4(0.0f, 0.0f, 0.5f, 0.0f),
            glm::vec4(move, default_height, z, 1.0f));
        tp.ambient_color = glm::vec4(0.0f);
        tp.diffuse_color = glm::vec4(move / 10.0f, 0.0f, z / 10.0f, 1.0f);
        tp.specular_color = glm::vec4(0.8f, 0.8f, 0.8f, 8.0f);
        teapot_ubos[last_teapot_index] = tp;
        teapot_times[last_teapot_index] = std::chrono::high_resolution_clock::now();
        last_teapot_index = (last_teapot_index + 1) % max_teapots;
      } else {
        z = -16; // z = 2
      }
    }
  }
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
void Application::on_key_pressed(GLFWwindow *window, int key, int scancode, int action, int mods) { camera.on_key_press(key); }