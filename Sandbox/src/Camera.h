#pragma once

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

class Camera
{
public:
  Camera(glm::vec3 position, glm::vec3 target, glm::vec3 worldUp);
  ~Camera();

  glm::vec3 Postion;
  glm::vec3 Forward;
  glm::vec3 Right;
  glm::vec3 Up;
  glm::vec3 WorldUp;

  glm::mat4 GetViewMatrix();
};
