#include "Light.h"

/**
 * @brief Constructs a Light object with specified position, color, and direction.
 */
Light::Light(const glm::vec3& pos, const glm::vec3& col, const glm::vec3& dir)
    : position(pos), color(col), direction(glm::normalize(dir))
{}

/**
 * @brief Returns the position of the light.
 */
glm::vec3 Light::getPosition() const
{
    return position;
}

/**
 * @brief Returns the color of the light.
 */
glm::vec3 Light::getColor() const
{
    return color;
}

/**
 * @brief Returns the direction of the light.
 */
glm::vec3 Light::getDirection() const
{
    return direction;
}

/**
 * @brief Sets the position of the light.
 */
void Light::setPosition(const glm::vec3& newPos)
{
    position = newPos;
}

/**
 * @brief Sets the color of the light.
 */
void Light::setColor(const glm::vec3& newColor)
{
    color = newColor;
}

/**
 * @brief Sets the direction of the light.
 */
void Light::setDirection(const glm::vec3& newDir)
{
    direction = glm::normalize(newDir);
}

/**
 * @brief Calculates the attenuation factor based on distance.
 *
 * @param distance The distance from the light source.
 * @return float The attenuation factor.
 */
float Light::calculateAttenuation(float distance) const
{
    constexpr float constant = 1.0f;
    constexpr float linear = 0.09f;
    constexpr float quadratic = 0.032f;
    return 1.0f / (constant + linear * distance + quadratic * (distance * distance));
}
