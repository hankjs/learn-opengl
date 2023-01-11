#include "Camera.h"
#include <iostream>

Camera::Camera(glm::vec3 position, glm::vec3 target, glm::vec3 worldUp) : MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
{
  Position = position;
  WorldUp = worldUp;
  Forward = glm::normalize(target - position);
  Right = glm::normalize(glm::cross(Forward, WorldUp));
  Up = glm::normalize(glm::cross(Forward, Right));
}

Camera::Camera(glm::vec3 position, float pitch, float yaw, glm::vec3 worldUp) : MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
{
  Position = position;
  WorldUp = worldUp;
  Pitch = pitch;
  Yaw = yaw;
  Forward.x = glm::cos(Pitch) * glm::sin(Yaw);
  Forward.y = glm::sin(Yaw);
  Forward.z = glm::cos(Pitch) * glm::cos(Yaw);
  Right = glm::normalize(glm::cross(Forward, WorldUp));
  Up = glm::normalize(glm::cross(Forward, Right));
}

Camera::~Camera()
{
}

glm::mat4 Camera::GetViewMatrix()
{
  return glm::lookAt(Position, Position + Forward, WorldUp);
}

// processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime)
{
  float velocity = MovementSpeed * deltaTime;
  if (direction == FORWARD)
    Position += Forward * velocity;
  if (direction == BACKWARD)
    Position -= Forward * velocity;
  if (direction == LEFT)
    Position -= Right * velocity;
  if (direction == RIGHT)
    Position += Right * velocity;
}

// processes input received from a mouse input system. Expects the offset value in both the x and y direction.
void Camera::ProcessMouseMovement(float xOffset, float yOffset, GLboolean constrainPitch)
{
  xOffset *= MouseSensitivity;
  yOffset *= MouseSensitivity;

  std::cout << "Before Yaw " << Yaw << std::endl;
  Yaw += xOffset;

  std::cout << "Yaw " << Yaw << std::endl;

  std::cout << "Before Pitch " << Pitch << std::endl;
  Pitch += yOffset;
  std::cout << "Pitch " << Pitch << std::endl;

  // make sure that when pitch is out of bounds, screen doesn't get flipped
  if (constrainPitch)
  {
    if (Pitch > 89.0f)
      Pitch = 89.0f;
    if (Pitch < -89.0f)
      Pitch = -89.0f;
  }

  // update Forward, Right and Up Vectors using the updated Euler angles
  updateCameraVectors();
}

// processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
void Camera::ProcessMouseScroll(float yOffset)
{
  Zoom -= (float)yOffset;
  if (Zoom < 1.0f)
    Zoom = 1.0f;
  if (Zoom > 45.0f)
    Zoom = 45.0f;
}

// calculates the forward vector from the Camera's (updated) Euler Angles
void Camera::updateCameraVectors()
{
  // calculate the new Forward vector
  Forward.x = glm::cos(Pitch) * glm::sin(Yaw);
  Forward.y = glm::sin(Yaw);
  Forward.z = glm::cos(Pitch) * glm::cos(Yaw);
  // also re-calculate the Right and Up vector
  Right = glm::normalize(glm::cross(Forward, WorldUp));
  Up = glm::normalize(glm::cross(Forward, Right));
}