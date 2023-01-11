#pragma once

#include <glad/glad.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.01f;
const float ZOOM = 45.0f;

class Camera
{
public:
    Camera(glm::vec3 position, glm::vec3 target, glm::vec3 worldUp);
    Camera(glm::vec3 position, float pitch, float yaw, glm::vec3 worldUp);
    ~Camera();

    glm::vec3 Position;
    glm::vec3 Forward;
    glm::vec3 Right;
    glm::vec3 Up;
    glm::vec3 WorldUp;

    // euler Angles
    float Yaw;
    float Pitch;

    // camera options
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;

    glm::mat4 GetViewMatrix();

    void ProcessKeyboard(Camera_Movement direction, float deltaTime);

    // processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void ProcessMouseMovement(float xOffset, float yOffset, GLboolean constrainPitch = true);

    // processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void ProcessMouseScroll(float yOffset);

private:
    // calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors();
};
