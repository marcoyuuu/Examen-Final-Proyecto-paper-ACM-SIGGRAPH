#ifndef PLANE_H
#define PLANE_H

#include "Shader.h"
#include "Mesh.h"
#include "Texture.h"
#include <memory>
#include <vector>

/**
 * @class Plane
 * @brief Representa un plano que actúa como terreno en la escena.
 *
 * La clase @c Plane permite la configuración y renderizado de un plano texturizado
 * para simular terrenos u otras superficies planas.
 */
class Plane {
public:
    /**
     * @brief Constructor por defecto.
     */
    Plane();

    /**
     * @brief Destructor por defecto.
     */
    ~Plane();

    /**
     * @brief Configura el plano cargando texturas y generando la geometría.
     */
    void Setup();

    /**
     * @brief Renderiza el plano.
     *
     * @param shader Shader a utilizar para renderizar.
     */
    void draw(const Shader &shader) const;

private:
    std::unique_ptr<Mesh> planeMesh; /**< Malla que representa el plano. */
    std::vector<Texture> textures;  /**< Texturas aplicadas al plano. */

    /**
     * @brief Genera vértices para el plano.
     *
     * @return std::vector<Vertex> Vértices generados.
     */
    std::vector<Vertex> generatePlaneVertices() const;

    /**
     * @brief Genera índices para el plano.
     *
     * @return std::vector<unsigned int> Índices generados.
     */
    std::vector<unsigned int> generatePlaneIndices() const;
};

#endif // PLANE_H
