// Lighthouse.h

#ifndef LIGHTHOUSE_H
#define LIGHTHOUSE_H

#include "Shader.h"
#include "Mesh.h"
#include "Texture.h"
#include <glm/glm.hpp>
#include <vector>
#include <memory>
#include <string>

/**
 * @brief Represents a textured lighthouse with basic lighting.
 */
class Lighthouse {
public:
    /**
     * @brief Constructor initializes mesh pointers.
     */
    Lighthouse();

    /**
     * @brief Destructor automatically handles mesh deletion via smart pointers.
     */
    ~Lighthouse();

    /**
     * @brief Sets up the lighthouse components by initializing meshes and loading textures.
     */
    void Setup();

    /**
     * @brief Renders the lighthouse with textures and lighting.
     *
     * @param shader The shader used for rendering.
     * @param time The current time to animate spotlight direction.
     */
    void Render(const Shader& shader, float time);

private:
    std::unique_ptr<Mesh> tower;    // Cylinder for the main tower
    std::unique_ptr<Mesh> roof;     // Cone for the roof
    std::unique_ptr<Mesh> beacon;   // Sphere for the beacon

    std::vector<Texture> towerTextures;  // Textures for the tower
    std::vector<Texture> roofTextures;   // Textures for the roof

    /**
     * @brief Generates vertices for a cylinder.
     *
     * @param radius Radius of the cylinder.
     * @param height Height of the cylinder.
     * @param sectorCount Number of sectors (slices) around the circumference.
     * @return std::vector<Vertex> Vector of generated vertices.
     */
    std::vector<Vertex> generateCylinderVertices(float radius, float height, int sectorCount) const;

    /**
     * @brief Generates indices for a cylinder.
     *
     * @param sectorCount Number of sectors (slices) around the circumference.
     * @return std::vector<unsigned int> Vector of generated indices.
     */
    std::vector<unsigned int> generateCylinderIndices(int sectorCount) const;

    /**
     * @brief Generates vertices for a cone.
     *
     * @param radius Base radius of the cone.
     * @param height Height of the cone.
     * @param sectorCount Number of sectors (slices) around the circumference.
     * @return std::vector<Vertex> Vector of generated vertices.
     */
    std::vector<Vertex> generateConeVertices(float radius, float height, int sectorCount) const;

    /**
     * @brief Generates indices for a cone.
     *
     * @param sectorCount Number of sectors (slices) around the circumference.
     * @return std::vector<unsigned int> Vector of generated indices.
     */
    std::vector<unsigned int> generateConeIndices(int sectorCount) const;

    /**
     * @brief Generates vertices for a sphere.
     *
     * @param radius Radius of the sphere.
     * @param sectorCount Number of sectors (slices) around the circumference.
     * @param stackCount Number of stacks (horizontal slices) from top to bottom.
     * @return std::vector<Vertex> Vector of generated vertices.
     */
    std::vector<Vertex> generateSphereVertices(float radius, int sectorCount, int stackCount) const;

    /**
     * @brief Generates indices for a sphere.
     *
     * @param sectorCount Number of sectors (slices) around the circumference.
     * @param stackCount Number of stacks (horizontal slices) from top to bottom.
     * @return std::vector<unsigned int> Vector of generated indices.
     */
    std::vector<unsigned int> generateSphereIndices(int sectorCount, int stackCount) const;

    /**
     * @brief Loads a texture from a given file path.
     *
     * @param path The file path to the texture image.
     * @param type The type of texture (e.g., "texture_diffuse").
     * @return Texture The loaded Texture object.
     */
    Texture loadTexture(const std::string& path, const std::string& type) const;
};

#endif // LIGHTHOUSE_H
