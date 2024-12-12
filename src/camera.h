// Camera.h

#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

/**
 * @brief Defines possible camera movement directions.
 */
enum class CameraMovement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

/**
 * @brief Represents a camera in 3D space.
 *
 * Handles view transformations and user input for camera movement.
 */
class Camera {
public:
    // Constructor with vectors
    Camera(glm::vec3 position = glm::vec3(0.0f, 5.0f, 20.0f), 
           glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
           float yaw = -90.0f, 
           float pitch = 0.0f);

    /**
     * @brief Returns the view matrix calculated using Euler Angles and the LookAt Matrix.
     * 
     * @return glm::mat4 The view matrix.
     */
    glm::mat4 GetViewMatrix() const;

    /**
     * @brief Processes input received from any keyboard-like input system.
     * 
     * @param direction The direction of movement.
     * @param deltaTime The time between the current frame and the last frame.
     */
    void ProcessKeyboard(CameraMovement direction, float deltaTime);

    /**
     * @brief Processes input received from a mouse input system.
     * 
     * @param xoffset The offset in the x-direction.
     * @param yoffset The offset in the y-direction.
     * @param constrainPitch Whether to constrain the pitch.
     */
    void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);

    /**
     * @brief Processes input received from a mouse scroll-wheel event.
     * 
     * @param yoffset The offset in the y-direction.
     */
    void ProcessMouseScroll(float yoffset);

    // Public member variables (consider making these private with getters/setters)
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;

    // Euler Angles
    float Yaw;
    float Pitch;

    // Camera options
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;

private:
    /**
     * @brief Calculates the front vector from the Camera's (updated) Euler Angles.
     */
    void updateCameraVectors();
};

#endif // CAMERA_H
