#ifndef LIGHT_H
#define LIGHT_H

#include <glm/glm.hpp>

/**
 * @brief Represents a light source in the scene.
 */
class Light {
public:
    /**
     * @brief Constructs a Light object with optional position, color, and direction.
     *
     * @param pos Position of the light.
     * @param col Color of the light.
     * @param dir Direction of the light (for directional lights).
     */
    Light(const glm::vec3& pos = glm::vec3(0.0f), 
          const glm::vec3& col = glm::vec3(1.0f), 
          const glm::vec3& dir = glm::vec3(0.0f, -1.0f, 0.0f));

    // Getters
    glm::vec3 getPosition() const;
    glm::vec3 getColor() const;
    glm::vec3 getDirection() const;

    // Setters
    void setPosition(const glm::vec3& newPos);
    void setColor(const glm::vec3& newColor);
    void setDirection(const glm::vec3& newDir);

    /**
     * @brief Calculates the attenuation factor based on distance.
     *
     * @param distance The distance from the light source.
     * @return float The attenuation factor.
     */
    float calculateAttenuation(float distance) const;

private:
    glm::vec3 position;
    glm::vec3 color;
    glm::vec3 direction;  // For directional lights
};

#endif // LIGHT_H
