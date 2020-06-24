#pragma once

#include <glm/glm.hpp>


class Camera {
private:

  static const float min_elevation;
  static const float max_elevation;
  static const float min_distance;
  static const float angle_sensitivity;
  static const float zoom_sensitivity;

  float angle_direction;

  float angle_elevation;

  /// Distance from (0,0,0), the point at which I look
  float distance;
  float speed = 0.2;

  /// Final position of the eye in world space coordinates, for LookAt or shaders
  glm::vec3 eye_position;

  glm::vec3 direction;

  /// Last X and Y coordinates of the mouse cursor
  int last_x, last_y;

  bool is_rotating;

  /// Recomputes 'eye_position' from 'angle_direction', 'angle_elevation', and 'distance'
  void update_eye_pos();
  void update_direction();

  float mid_y = 5.0f;

public:
  Camera();

  /// Call when the user presses or releases a mouse button (see glfwSetMouseButtonCallback)
  void on_mouse_button(int button, int action, int mods);

  /// Call when the user moves with the mouse cursor (see glfwSetCursorPosCallback)
  void on_mouse_move(double x, double y);

  void on_key_press(int key);

  /// Returns the position of the eye in world space coordinates
  glm::vec3 get_eye_position() const;

  glm::vec3 get_direction() const;
};
