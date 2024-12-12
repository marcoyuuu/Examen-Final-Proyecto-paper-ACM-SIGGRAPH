// Mesh.h

#ifndef MESH_H
#define MESH_H

#include <glad/glad.h> // Must be included before GLFW or other OpenGL headers
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include "Shader.h"
#include "Texture.h"

/**
 * @brief Structure to hold vertex data.
 */
struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec3 Color;
    glm::vec2 TexCoords;
};

/**
 * @brief Represents a mesh with vertex and index data, along with associated textures.
 */
class Mesh {
public:
    /**
     * @brief Constructs a Mesh object with vertices, indices, and textures.
     *
     * @param vertices Vector of Vertex structures.
     * @param indices Vector of indices for indexed drawing.
     * @param textures Vector of Texture objects associated with the mesh.
     */
    Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, std::vector<Texture>&& textures);

    // Delete copy constructor and assignment to prevent copying
    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;

    // Allow move semantics
    Mesh(Mesh&& other) noexcept;
    Mesh& operator=(Mesh&& other) noexcept;

    /**
     * @brief Destructor to clean up OpenGL resources.
     */
    ~Mesh();

    /**
     * @brief Renders the mesh using the provided shader.
     *
     * @param shader Shader program to use for rendering.
     */
    void Draw(const Shader& shader) const;

private:
    // Render data
    GLuint VAO, VBO, EBO;
    GLsizei indexCount;
    std::vector<Texture> textures;

    /**
     * @brief Initializes all the buffer objects/arrays.
     *
     * @param vertices Vector of Vertex structures.
     * @param indices Vector of indices for indexed drawing.
     */
    void setupMesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
};

#endif // MESH_H
