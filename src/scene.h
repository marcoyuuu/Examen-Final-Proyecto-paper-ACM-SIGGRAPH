#ifndef SCENE_H
#define SCENE_H

#include "Camera.h"
#include "Shader.h"
#include "Light.h"
#include "Mesh.h"
#include "Plane.h"
#include "Lighthouse.h"
#include "Texture.h"
#include "Constants.h"
#include <vector>
#include <string>
#include <memory>
#include <glm/glm.hpp>

/**
 * @struct PointLightData
 * @brief Contiene los datos necesarios para describir una luz puntual.
 *
 * Incluye posición, componentes ambiental, difusa, especular y parámetros de atenuación.
 */
struct PointLightData {
    glm::vec3 position;   /**< Posición de la luz puntual en el mundo */
    glm::vec3 ambient;    /**< Componente ambiental de la luz */
    glm::vec3 diffuse;    /**< Componente difusa de la luz */
    glm::vec3 specular;   /**< Componente especular de la luz */
    float constant;       /**< Factor constante de atenuación */
    float linear;         /**< Factor lineal de atenuación */
    float quadratic;      /**< Factor cuadrático de atenuación */
};

/**
 * @struct DirectionalLightData
 * @brief Contiene los datos necesarios para describir una luz direccional.
 *
 * Incluye dirección y componentes ambiental, difusa y especular.
 */
struct DirectionalLightData {
    glm::vec3 direction;  /**< Dirección de la luz direccional */
    glm::vec3 ambient;    /**< Componente ambiental */
    glm::vec3 diffuse;    /**< Componente difusa */
    glm::vec3 specular;   /**< Componente especular */
};

/**
 * @class Scene
 * @brief Representa toda la escena a renderizar.
 *
 * La escena contiene un skybox, un faro (lighthouse), un plano (terreno),
 * múltiples luces (direccional, puntuales y spotlight) y facilita su configuración y renderizado.
 */
class Scene {
public:
    /**
     * @brief Constructor por defecto. Inicializa el spotlight y prepara buffers del skybox.
     */
    Scene();

    /**
     * @brief Destructor. Limpia VAOs y VBOs del skybox.
     */
    ~Scene();

    /**
     * @brief Configura la escena: carga skybox, configura luces, crea el faro y el plano.
     */
    void Setup();

    /**
     * @brief Renderiza la escena completa, incluyendo skybox, lighthouse, plano y objetos adicionales.
     * @param shader Shader principal de la escena (iluminación).
     * @param camera Cámara activa desde la que se ve la escena.
     * @param skyboxShader Shader para el skybox.
     */
    void Render(Shader &shader, const Camera &camera, Shader &skyboxShader);

private:
    std::vector<std::unique_ptr<Mesh>> meshes;   /**< Lista de mallas adicionales en la escena */
    Light spotlight;                             /**< Spotlight principal (faro) */
    unsigned int skyboxTexture;                  /**< Textura cubemap del skybox */
    unsigned int skyboxVAO, skyboxVBO;           /**< VAO y VBO para el skybox */
    Plane groundPlane;                           /**< Plano del terreno */
    std::unique_ptr<Lighthouse> lighthouse;      /**< Faro principal en la escena */

    DirectionalLightData dirLight;               /**< Luz direccional (ej: sol) */
    std::vector<PointLightData> pointLights;     /**< Luces puntuales en la escena */

    /**
     * @brief Verifica si una esfera está dentro del frustum de la cámara.
     * @param center Centro de la esfera.
     * @param radius Radio de la esfera.
     * @param vpMatrix Matriz View-Projection actual.
     * @return true si la esfera está dentro, false si no.
     */
    bool isSphereInFrustum(const glm::vec3 &center, float radius, const glm::mat4 &vpMatrix) const;

    /**
     * @brief Carga un cubemap a partir de una lista de rutas de textura.
     * @param faces Vector con las rutas de las texturas para cada cara del cubemap.
     * @return ID de textura OpenGL.
     */
    unsigned int loadCubemap(const std::vector<std::string>& faces);

    /**
     * @brief Crea el VAO y VBO para el skybox.
     */
    void createSkybox();
};

#endif // SCENE_H
