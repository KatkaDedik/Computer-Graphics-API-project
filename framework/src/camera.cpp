#include "camera.hpp"

#include <GLFW/glfw3.h>
const float Camera::min_elevation = -1.5f;
const float Camera::max_elevation = 1.5f;
const float Camera::min_distance = 1.0f;
const float Camera::angle_sensitivity = 0.003f;
const float Camera::zoom_sensitivity = 0.003f;

Camera::Camera()
    : angle_direction(0.0f), angle_elevation(0.26f), distance(16.0f), last_x(0), last_y(5), is_rotating(false) {
  update_eye_pos();
  update_direction();
  eye_position.z += 12;
}

void Camera::update_eye_pos() {
  eye_position.x = distance * cosf(angle_elevation) * -sinf(angle_direction);
  eye_position.y = mid_y + distance * sinf(angle_elevation);
  eye_position.z = distance * cosf(angle_elevation) * cosf(angle_direction);
}

void Camera::update_direction() {
  direction.x = distance * cosf(angle_elevation) * -sinf(angle_direction);
  direction.y = mid_y + distance * sinf(angle_elevation);
  direction.z = distance * cosf(angle_elevation) * cosf(angle_direction);
}

void Camera::on_mouse_button(int button, int action, int mods) {
  // Left mouse button affects the angles
  if (button == GLFW_MOUSE_BUTTON_LEFT) {
    if (action == GLFW_PRESS) {
      is_rotating = true;
    } else {
      is_rotating = false;
    }
  }
}

void Camera::on_mouse_move(double x, double y) {
  float dx = float(x - last_x);
  float dy = float(y - last_y);
  last_x = static_cast<int>(x);
  last_y = static_cast<int>(y);

  if (is_rotating) {
    angle_direction += dx * angle_sensitivity;
    angle_elevation += dy * angle_sensitivity;

    // Clamp the results
    if (angle_elevation > max_elevation)
      angle_elevation = max_elevation;
    if (angle_elevation < min_elevation)
      angle_elevation = min_elevation;
  }

 
  update_direction();
}

void Camera::on_key_press(int key) {
  if (GLFW_KEY_UP == key) {
    eye_position.z -= speed;
    direction.z -= speed;
  }
  if (GLFW_KEY_DOWN == key) {
    eye_position.z += speed;
    direction.z += speed;
  } 
  if (GLFW_KEY_RIGHT == key) {

    eye_position.x += speed;
    direction.x += speed;
  }
  // Strafe left
  if (GLFW_KEY_LEFT == key) {
    eye_position.x -= speed;
    direction.x -= speed;
  }
        
}

glm::vec3 Camera::get_eye_position() const { return eye_position; }
glm::vec3 Camera::get_direction() const { return direction; }
