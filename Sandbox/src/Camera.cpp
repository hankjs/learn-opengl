#include "Camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 target, glm::vec3 worldUp)
{
  Postion = position;
  WorldUp = worldUp;
  Forward = glm::normalize(target - position);
  Right = glm::normalize(glm::cross(Forward, WorldUp));
  Up = glm::normalize(glm::cross(Forward, Right));
}

Camera::Camera(glm::vec3 position, float pitch, float yaw, glm::vec3 worldUp)
{
  Postion = position;
  WorldUp = worldUp;
  Forward.x = glm::cos(pitch) * glm::sin(yaw);
  Forward.y = glm::sin(yaw);
  Forward.z = glm::cos(pitch) * glm::cos(yaw);
  Right = glm::normalize(glm::cross(Forward, WorldUp));
  Up = glm::normalize(glm::cross(Forward, Right));
}

Camera::~Camera()
{
}

glm::mat4 Camera::GetViewMatrix()
{
  return glm::lookAt(Postion, Postion + Forward, WorldUp);
}
