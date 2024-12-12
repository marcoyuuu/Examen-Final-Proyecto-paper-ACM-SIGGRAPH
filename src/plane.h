// Plane.h

#ifndef PLANE_H
#define PLANE_H

#include "Shader.h"
#include "Mesh.h"
#include "Texture.h" // Ensure Texture.h is included
#include <memory>
#include <vector>

/**
 * @brief Represents a ground plane in the scene.
 */
class Plane {
public:
    Plane();
    ~Plane();

    /**
     * @brief Sets up the ground plane by loading textures and initializing the mesh.
     */
    void Setup();

    /**
     * @brief Renders the ground plane.
     *
     * @param shader The shader used for rendering.
     */
    void draw(const Shader &shader) const;

private:
    std::unique_ptr<Mesh> planeMesh;
    std::vector<Texture> textures; // To store plane's textures

    /**
     * @brief Generates vertices for a large ground plane.
     *
     * @return std::vector<Vertex> The list of vertices.
     */
    std::vector<Vertex> generatePlaneVertices() const;

    /**
     * @brief Generates indices for a quad ground plane.
     *
     * @return std::vector<unsigned int> The list of indices.
     */
    std::vector<unsigned int> generatePlaneIndices() const;
};

#endif // PLANE_H
