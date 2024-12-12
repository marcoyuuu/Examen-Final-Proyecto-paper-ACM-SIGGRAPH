#ifndef SCENE_H
#define SCENE_H

#include "Camera.h"
#include "Shader.h"
#include "Light.h"
#include "Mesh.h"
#include "Plane.h"
#include "Lighthouse.h"
#include "Texture.h"
#include "Constants.h"  // For WINDOW_WIDTH, WINDOW_HEIGHT
#include <vector>
#include <string>
#include <memory>
#include <glm/glm.hpp>

/**
 * @brief Holds data for a point light in the scene.
 */
struct PointLightData {
    glm::vec3 position;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float constant;
    float linear;
    float quadratic;
};

/**
 * @brief Holds data for a directional light in the scene.
 */
struct DirectionalLightData {
    glm::vec3 direction;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
};

/**
 * @brief Represents the entire scene to be rendered.
 *
 * Handles setup, rendering, and resource management for all scene components:
 * - Skybox
 * - Lighthouse
 * - Ground plane
 * - Multiple lights (directional, point lights, and spotlight)
 */
class Scene {
public:
    Scene();
    ~Scene();

    /**
     * @brief Sets up the scene by initializing skybox, lighthouse, plane, and lights.
     */
    void Setup();

    /**
     * @brief Renders the entire scene with given shaders and camera.
     *
     * @param shader The main scene shader (phong lighting).
     * @param camera The active camera.
     * @param skyboxShader The shader used for rendering the skybox.
     */
    void Render(Shader &shader, const Camera &camera, Shader &skyboxShader);

private:
    std::vector<std::unique_ptr<Mesh>> meshes;   // Additional meshes if any
    Light spotlight;                             // Spotlight (like the lighthouse beacon)
    unsigned int skyboxTexture;
    unsigned int skyboxVAO, skyboxVBO;
    Plane groundPlane;                           // The ground plane
    std::unique_ptr<Lighthouse> lighthouse;      // The lighthouse

    // Lights
    DirectionalLightData dirLight;
    std::vector<PointLightData> pointLights;

    // Frustum culling helper
    bool isSphereInFrustum(const glm::vec3 &center, float radius, const glm::mat4 &vpMatrix) const;

    // Skybox helper
    unsigned int loadCubemap(const std::vector<std::string>& faces);
    void createSkybox();
};

#endif // SCENE_H
