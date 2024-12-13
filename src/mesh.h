#ifndef MESH_H
#define MESH_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include "Shader.h"
#include "Texture.h"

/**
 * @struct Vertex
 * @brief Almacena la información de un vértice (posición, normal, color, coordenadas de textura).
 */
struct Vertex {
    glm::vec3 Position;  /**< Posición del vértice */
    glm::vec3 Normal;    /**< Normal del vértice */
    glm::vec3 Color;     /**< Color del vértice */
    glm::vec2 TexCoords; /**< Coordenadas de textura del vértice */
};

/**
 * @class Mesh
 * @brief Representa una malla con vértices, índices y texturas.
 *
 * La clase @c Mesh encapsula la creación de buffers de OpenGL (VBO, VAO, EBO)
 * y ofrece una función @c Draw para renderizar la geometría con un shader dado.
 */
class Mesh {
public:
    /**
     * @brief Constructor de Mesh.
     * @param vertices Vector de vértices.
     * @param indices Vector de índices para dibujo indexado.
     * @param textures Vector de texturas asociadas a la malla.
     */
    Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, std::vector<Texture>&& textures);

    // Delete copy constructor and assignment to prevent copying
    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;

    // Move semantics
    Mesh(Mesh&& other) noexcept;
    Mesh& operator=(Mesh&& other) noexcept;

    /**
     * @brief Destructor. Libera los recursos de OpenGL.
     */
    ~Mesh();

    /**
     * @brief Dibuja la malla usando el shader proporcionado.
     * @param shader Shader a utilizar.
     */
    void Draw(const Shader& shader) const;

private:
    GLuint VAO, VBO, EBO;   /**< Identificadores de buffers OpenGL */
    GLsizei indexCount;     /**< Cantidad de índices de la malla */
    std::vector<Texture> textures; /**< Texturas asociadas a la malla */

    /**
     * @brief Inicializa los buffers (VAO, VBO, EBO) y configura las entradas de vértice.
     * @param vertices Lista de vértices.
     * @param indices Lista de índices.
     */
    void setupMesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
};

#endif // MESH_H
