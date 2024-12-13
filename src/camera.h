#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

/**
 * @enum CameraMovement
 * @brief Posibles direcciones de movimiento para la cámara.
 */
enum class CameraMovement {
    FORWARD,   /**< Avanzar hacia adelante */
    BACKWARD,  /**< Avanzar hacia atrás */
    LEFT,      /**< Desplazar a la izquierda */
    RIGHT      /**< Desplazar a la derecha */
};

/**
 * @class Camera
 * @brief Representa una cámara en espacio 3D.
 *
 * La clase @c Camera gestiona transformaciones de vista y entrada del usuario
 * para mover la cámara en una escena 3D. Ofrece métodos para manejar movimiento
 * por teclado, entrada de ratón y zoom mediante scroll.
 */
class Camera {
public:
    /**
     * @brief Constructor que especifica posición e inclinación inicial de la cámara.
     * @param position Posición inicial de la cámara.
     * @param up Vector Up mundial.
     * @param yaw Ángulo yaw inicial (en grados).
     * @param pitch Ángulo pitch inicial (en grados).
     */
    Camera(glm::vec3 position = glm::vec3(0.0f, 5.0f, 20.0f), 
           glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
           float yaw = -90.0f, 
           float pitch = 0.0f);

    /**
     * @brief Obtiene la matriz de vista calculada usando las propiedades actuales de la cámara.
     * @return glm::mat4 Matriz de vista.
     */
    glm::mat4 GetViewMatrix() const;

    /**
     * @brief Procesa la entrada del teclado para mover la cámara en una dirección dada.
     * @param direction Dirección del movimiento (ver @ref CameraMovement).
     * @param deltaTime Tiempo transcurrido desde el último frame, para movimiento constante independiente del frame rate.
     */
    void ProcessKeyboard(CameraMovement direction, float deltaTime);

    /**
     * @brief Procesa el movimiento del ratón para rotar la cámara.
     * @param xoffset Desplazamiento en el eje X del ratón.
     * @param yoffset Desplazamiento en el eje Y del ratón.
     * @param constrainPitch Indica si se restringe el rango de pitch para evitar invertir la vista.
     */
    void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);

    /**
     * @brief Procesa la entrada de la rueda del ratón para ajustar el zoom de la cámara.
     * @param yoffset Desplazamiento en la rueda del ratón.
     */
    void ProcessMouseScroll(float yoffset);

    /// Posición de la cámara en el mundo.
    glm::vec3 Position;
    /// Vector frontal de la cámara.
    glm::vec3 Front;
    /// Vector Up actual de la cámara.
    glm::vec3 Up;
    /// Vector Right calculado a partir de Front y WorldUp.
    glm::vec3 Right;
    /// Vector Up del mundo (generalmente (0,1,0)).
    glm::vec3 WorldUp;

    /// Ángulo yaw de la cámara (en grados).
    float Yaw;
    /// Ángulo pitch de la cámara (en grados).
    float Pitch;

    /// Velocidad de movimiento (unidades/segundo).
    float MovementSpeed;
    /// Sensibilidad del ratón para rotaciones.
    float MouseSensitivity;
    /// Ángulo de zoom (FOV) de la cámara.
    float Zoom;

private:
    /**
     * @brief Actualiza los vectores de la cámara (Front, Right, Up) a partir de Yaw y Pitch.
     */
    void updateCameraVectors();
};

#endif // CAMERA_H
