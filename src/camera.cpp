// Camera.cpp

#include "Camera.h"

// Constructor
Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
    : Front(glm::vec3(0.0f, 0.0f, -1.0f)),
      MovementSpeed(10.0f),
      MouseSensitivity(0.1f),
      Zoom(45.0f),
      Position(position),
      WorldUp(up),
      Yaw(yaw),
      Pitch(pitch)
{
    updateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix() const
{
    return glm::lookAt(Position, Position + Front, Up);
}

void Camera::ProcessKeyboard(CameraMovement direction, float deltaTime)
{
    float velocity = MovementSpeed * deltaTime;
    switch(direction)
    {
        case CameraMovement::FORWARD:
            Position += Front * velocity;
            break;
        case CameraMovement::BACKWARD:
            Position -= Front * velocity;
            break;
        case CameraMovement::LEFT:
            Position -= Right * velocity;
            break;
        case CameraMovement::RIGHT:
            Position += Right * velocity;
            break;
        default:
            break;
    }
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch)
{
    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;

    Yaw   += xoffset;
    Pitch += yoffset;

    // Make sure that when pitch is out of bounds, screen doesn't get flipped
    if(constrainPitch)
    {
        if(Pitch > 89.0f)
            Pitch = 89.0f;
        if(Pitch < -89.0f)
            Pitch = -89.0f;
    }

    // Update Front, Right and Up Vectors using the updated Euler angles
    updateCameraVectors();
}

void Camera::ProcessMouseScroll(float yoffset)
{
    Zoom -= yoffset;
    if(Zoom < 1.0f)
        Zoom = 1.0f;
    if(Zoom > 45.0f)
        Zoom = 45.0f;
}

void Camera::updateCameraVectors()
{
    // Calculate the new Front vector
    glm::vec3 front;
    front.x = std::cos(glm::radians(Yaw)) * std::cos(glm::radians(Pitch));
    front.y = std::sin(glm::radians(Pitch));
    front.z = std::sin(glm::radians(Yaw)) * std::cos(glm::radians(Pitch));
    Front = glm::normalize(front);
    // Also re-calculate the Right and Up vector
    Right = glm::normalize(glm::cross(Front, WorldUp));  
    Up    = glm::normalize(glm::cross(Right, Front));
}
