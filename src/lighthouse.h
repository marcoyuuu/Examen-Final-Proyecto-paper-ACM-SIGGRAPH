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
 * @class Lighthouse
 * @brief Representa un faro texturizado con componentes básicos de iluminación.
 *
 * La clase @c Lighthouse gestiona la geometría y texturas de un faro, incluyendo su torre, 
 * techo y beacon. También maneja la configuración de una luz giratoria simulando el beacon.
 */
class Lighthouse {
public:
    /**
     * @brief Constructor que inicializa punteros inteligentes para las partes del faro.
     */
    Lighthouse();

    /**
     * @brief Destructor que limpia automáticamente los recursos de las mallas.
     */
    ~Lighthouse();

    /**
     * @brief Configura el faro cargando texturas y generando geometría.
     */
    void Setup();

    /**
     * @brief Renderiza el faro con iluminación y texturas aplicadas.
     *
     * @param shader El shader a utilizar para renderizar.
     * @param time El tiempo actual para animar la dirección del spotlight.
     */
    void Render(const Shader& shader, float time);

private:
    std::unique_ptr<Mesh> tower;    /**< Malla que representa la torre del faro. */
    std::unique_ptr<Mesh> roof;     /**< Malla que representa el techo del faro. */
    std::unique_ptr<Mesh> beacon;   /**< Malla que representa el beacon del faro. */

    std::vector<Texture> towerTextures; /**< Texturas aplicadas a la torre. */
    std::vector<Texture> roofTextures;  /**< Texturas aplicadas al techo. */

    /**
     * @brief Genera vértices para un cilindro.
     *
     * @param radius Radio del cilindro.
     * @param height Altura del cilindro.
     * @param sectorCount Número de sectores alrededor de la circunferencia.
     * @return std::vector<Vertex> Vértices generados.
     */
    std::vector<Vertex> generateCylinderVertices(float radius, float height, int sectorCount) const;

    /**
     * @brief Genera índices para un cilindro.
     *
     * @param sectorCount Número de sectores alrededor de la circunferencia.
     * @return std::vector<unsigned int> Índices generados.
     */
    std::vector<unsigned int> generateCylinderIndices(int sectorCount) const;

    /**
     * @brief Genera vértices para un cono.
     *
     * @param radius Radio base del cono.
     * @param height Altura del cono.
     * @param sectorCount Número de sectores alrededor de la circunferencia.
     * @return std::vector<Vertex> Vértices generados.
     */
    std::vector<Vertex> generateConeVertices(float radius, float height, int sectorCount) const;

    /**
     * @brief Genera índices para un cono.
     *
     * @param sectorCount Número de sectores alrededor de la circunferencia.
     * @return std::vector<unsigned int> Índices generados.
     */
    std::vector<unsigned int> generateConeIndices(int sectorCount) const;

    /**
     * @brief Genera vértices para una esfera.
     *
     * @param radius Radio de la esfera.
     * @param sectorCount Número de sectores (cortes verticales).
     * @param stackCount Número de stacks (cortes horizontales).
     * @return std::vector<Vertex> Vértices generados.
     */
    std::vector<Vertex> generateSphereVertices(float radius, int sectorCount, int stackCount) const;

    /**
     * @brief Genera índices para una esfera.
     *
     * @param sectorCount Número de sectores (cortes verticales).
     * @param stackCount Número de stacks (cortes horizontales).
     * @return std::vector<unsigned int> Índices generados.
     */
    std::vector<unsigned int> generateSphereIndices(int sectorCount, int stackCount) const;

    /**
     * @brief Carga una textura desde un archivo.
     *
     * @param path Ruta al archivo de textura.
     * @param type Tipo de textura (por ejemplo, "texture_diffuse").
     * @return Texture Textura cargada.
     */
    Texture loadTexture(const std::string& path, const std::string& type) const;
};

#endif // LIGHTHOUSE_H
