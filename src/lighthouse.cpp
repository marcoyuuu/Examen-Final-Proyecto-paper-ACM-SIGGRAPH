// Lighthouse.cpp

#define _USE_MATH_DEFINES
#include <cmath>
#include "Lighthouse.h"
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>
#include <iostream>

// Constructor initializes mesh pointers.
Lighthouse::Lighthouse()
    : tower(nullptr), roof(nullptr), beacon(nullptr) {}

// Destructor automatically handles mesh deletion via smart pointers.
Lighthouse::~Lighthouse()
{
    // Smart pointers automatically clean up.
}

// Loads a texture from a given file path and ensures it is loaded into OpenGL.
Texture Lighthouse::loadTexture(const std::string& path, const std::string& type) const
{
    Texture tex(path, type);
    if (!tex.load()) {
        std::cerr << "Failed to load texture: " << path << std::endl;
    }
    return tex;
}

// Sets up the lighthouse by generating and initializing meshes and loading textures.
void Lighthouse::Setup()
{
    // Load textures for the tower.
    Texture towerDiffuse = loadTexture("C:/Users/yumar/OneDrive/Desktop/COMP 4046/OpenGL/main7/assets/textures/lighthouse/seaworn_sandstone_brick_diff_2k.jpg", "texture_diffuse");
    Texture towerNormal = loadTexture("C:/Users/yumar/OneDrive/Desktop/COMP 4046/OpenGL/main7/assets/textures/lighthouse/seaworn_sandstone_brick_nor_gl_2k.exr", "texture_normal");
    Texture towerRoughness = loadTexture("C:/Users/yumar/OneDrive/Desktop/COMP 4046/OpenGL/main7/assets/textures/lighthouse/seaworn_sandstone_brick_rough_2k.exr", "texture_roughness");
    towerTextures.push_back(std::move(towerDiffuse));
    towerTextures.push_back(std::move(towerNormal));
    towerTextures.push_back(std::move(towerRoughness));

    // Load textures for the roof.
    Texture roofDiffuse = loadTexture("C:/Users/yumar/OneDrive/Desktop/COMP 4046/OpenGL/main7/assets/textures/lighthouse/seaworn_sandstone_brick_diff_2k.jpg", "texture_diffuse");
    Texture roofNormal = loadTexture("C:/Users/yumar/OneDrive/Desktop/COMP 4046/OpenGL/main7/assets/textures/lighthouse/seaworn_sandstone_brick_nor_gl_2k.exr", "texture_normal");
    Texture roofRoughness = loadTexture("C:/Users/yumar/OneDrive/Desktop/COMP 4046/OpenGL/main7/assets/textures/lighthouse/seaworn_sandstone_brick_rough_2k.exr", "texture_roughness");
    roofTextures.push_back(std::move(roofDiffuse));
    roofTextures.push_back(std::move(roofNormal));
    roofTextures.push_back(std::move(roofRoughness));

    // Generate tower mesh.
    auto towerVertices = generateCylinderVertices(1.0f, 10.0f, 36);
    auto towerIndices = generateCylinderIndices(36);
    tower = std::make_unique<Mesh>(towerVertices, towerIndices, std::move(towerTextures));

    // Generate roof mesh.
    auto roofVertices = generateConeVertices(1.5f, 3.0f, 36);
    auto roofIndices = generateConeIndices(36);
    roof = std::make_unique<Mesh>(roofVertices, roofIndices, std::move(roofTextures));

    // Generate beacon mesh.
    auto beaconVertices = generateSphereVertices(0.5f, 36, 18);
    auto beaconIndices = generateSphereIndices(36, 18);
    beacon = std::make_unique<Mesh>(beaconVertices, beaconIndices, std::vector<Texture>()); // No textures for beacon.
}

// Renders the lighthouse with textures and lighting.
void Lighthouse::Render(const Shader& shader, float time)
{
    // === Render Tower ===
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 5.0f, 0.0f)); // Position at (0,5,0)
    shader.setMat4("model", model);
    tower->Draw(shader);

    // === Render Roof ===
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 10.0f, 0.0f)); // Position at (0,10,0)
    shader.setMat4("model", model);
    roof->Draw(shader);

    // === Render Beacon ===
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 12.0f, 0.0f)); // Position at (0,12,0)
    shader.setMat4("model", model);
    beacon->Draw(shader);

    // === Spotlight (Beacon Light) Setup ===
    // Spotlight direction rotates around the Y-axis to simulate rotation.
    float angle = time * glm::radians(45.0f); // 45 degrees per second.
    glm::vec3 spotLightPos = glm::vec3(0.0f, 12.0f, 0.0f); // Fixed at beacon position.
    glm::vec3 spotLightDir = glm::normalize(glm::vec3(std::cos(angle), -1.0f, std::sin(angle))); // Rotating direction.

    // Update spotlight uniforms for dynamic direction.
    shader.setVec3("spotLight.position", spotLightPos);
    shader.setVec3("spotLight.direction", spotLightDir);
}

// Generates vertices for a cylinder.
std::vector<Vertex> Lighthouse::generateCylinderVertices(float radius, float height, int sectorCount) const
{
    std::vector<Vertex> vertices;
    float sectorStep = 2 * M_PI / sectorCount;
    float halfHeight = height / 2.0f;

    // Generate vertices for the top and bottom circles.
    for(int i = 0; i <= sectorCount; ++i)
    {
        float sectorAngle = i * sectorStep;
        float x = radius * std::cos(sectorAngle);
        float z = radius * std::sin(sectorAngle);

        // Top vertex.
        Vertex topVertex;
        topVertex.Position = glm::vec3(x, halfHeight, z);
        topVertex.Normal = glm::vec3(x, 0.0f, z); // Normal pointing outward.
        topVertex.Color = glm::vec3(1.0f); // White color or set as needed
        topVertex.TexCoords = glm::vec2(static_cast<float>(i) / sectorCount, 1.0f); // U varies, V=1 for top.
        vertices.push_back(topVertex);

        // Bottom vertex.
        Vertex bottomVertex;
        bottomVertex.Position = glm::vec3(x, -halfHeight, z);
        bottomVertex.Normal = glm::vec3(x, 0.0f, z); // Normal pointing outward.
        bottomVertex.Color = glm::vec3(1.0f); // White color or set as needed
        bottomVertex.TexCoords = glm::vec2(static_cast<float>(i) / sectorCount, 0.0f); // U varies, V=0 for bottom.
        vertices.push_back(bottomVertex);
    }

    return vertices;
}

// Generates indices for a cylinder.
std::vector<unsigned int> Lighthouse::generateCylinderIndices(int sectorCount) const
{
    std::vector<unsigned int> indices;
    int k1, k2;

    for(int i = 0; i < sectorCount; ++i)
    {
        k1 = i * 2;     // Top vertex of current sector.
        k2 = k1 + 1;    // Bottom vertex of current sector.

        // Two triangles per sector.
        indices.push_back(k1);
        indices.push_back(k2);
        indices.push_back(k1 + 2);

        indices.push_back(k2);
        indices.push_back(k2 + 2);
        indices.push_back(k1 + 2);
    }

    return indices;
}

// Generates vertices for a cone.
std::vector<Vertex> Lighthouse::generateConeVertices(float radius, float height, int sectorCount) const
{
    std::vector<Vertex> vertices;
    float sectorStep = 2 * M_PI / sectorCount;

    // Tip of the cone.
    Vertex tipVertex;
    tipVertex.Position = glm::vec3(0.0f, height / 2.0f, 0.0f);
    tipVertex.Normal = glm::vec3(0.0f, 1.0f, 0.0f); // Normal can be adjusted for smooth shading.
    tipVertex.Color = glm::vec3(1.0f); // White color or set as needed
    tipVertex.TexCoords = glm::vec2(0.5f, 1.0f); // Center of the texture.
    vertices.push_back(tipVertex);

    // Base vertices.
    for(int i = 0; i <= sectorCount; ++i)
    {
        float sectorAngle = i * sectorStep;
        float x = radius * std::cos(sectorAngle);
        float z = radius * std::sin(sectorAngle);

        Vertex baseVertex;
        baseVertex.Position = glm::vec3(x, -height / 2.0f, z);
        baseVertex.Normal = glm::vec3(x, 0.0f, z); // Approximate normal.
        baseVertex.Color = glm::vec3(1.0f); // White color or set as needed
        baseVertex.TexCoords = glm::vec2((std::cos(sectorAngle) + 1.0f) * 0.5f, (std::sin(sectorAngle) + 1.0f) * 0.5f); // Mapping to texture.
        vertices.push_back(baseVertex);
    }

    return vertices;
}

// Generates indices for a cone.
std::vector<unsigned int> Lighthouse::generateConeIndices(int sectorCount) const
{
    std::vector<unsigned int> indices;

    // Tip vertex index is 0.
    for(int i = 1; i <= sectorCount; ++i)
    {
        indices.push_back(0);    // Tip of the cone.
        indices.push_back(i);    // Current base vertex.
        indices.push_back(i + 1); // Next base vertex.
    }

    return indices;
}

// Generates vertices for a sphere.
std::vector<Vertex> Lighthouse::generateSphereVertices(float radius, int sectorCount, int stackCount) const
{
    std::vector<Vertex> vertices;
    float x, y, z, xy;                              // Vertex position.
    float nx, ny, nz, lengthInv = 1.0f / radius;    // Vertex normal.
    float s, t;                                     // Vertex texture coordinates.

    float sectorStep = 2 * M_PI / sectorCount;
    float stackStep = M_PI / stackCount;
    float sectorAngle, stackAngle;

    for(int i = 0; i <= stackCount; ++i)
    {
        stackAngle = M_PI / 2 - i * stackStep;        // Starting from pi/2 to -pi/2.
        xy = radius * std::cos(stackAngle);             // r * cos(u).
        y = radius * std::sin(stackAngle);              // r * sin(u).

        // Add (sectorCount+1) vertices per stack.
        for(int j = 0; j <= sectorCount; ++j)
        {
            sectorAngle = j * sectorStep;           // Starting from 0 to 2pi.

            // Vertex position.
            x = xy * std::cos(sectorAngle);             // r * cos(u) * cos(v).
            z = xy * std::sin(sectorAngle);             // r * cos(u) * sin(v).
            Vertex vertex;
            vertex.Position = glm::vec3(x, y, z);

            // Normalized vertex normal.
            nx = x * lengthInv;
            ny = y * lengthInv;
            nz = z * lengthInv;
            vertex.Normal = glm::vec3(nx, ny, nz);

            // Vertex texture coordinates between [0, 1].
            s = static_cast<float>(j) / sectorCount;
            t = static_cast<float>(i) / stackCount;
            vertex.TexCoords = glm::vec2(s, t);

            vertex.Color = glm::vec3(1.0f); // White color or set as needed

            vertices.push_back(vertex);
        }
    }

    return vertices;
}

// Generates indices for a sphere.
std::vector<unsigned int> Lighthouse::generateSphereIndices(int sectorCount, int stackCount) const
{
    std::vector<unsigned int> indices;
    int k1, k2;

    for(int i = 0; i < stackCount; ++i)
    {
        k1 = i * (sectorCount + 1);     // Beginning of current stack.
        k2 = k1 + sectorCount + 1;      // Beginning of next stack.

        for(int j = 0; j < sectorCount; ++j, ++k1, ++k2)
        {
            // 2 triangles per sector excluding first and last stacks.
            if(i != 0)
            {
                indices.push_back(k1);
                indices.push_back(k2);
                indices.push_back(k1 + 1);
            }

            if(i != (stackCount - 1))
            {
                indices.push_back(k1 + 1);
                indices.push_back(k2);
                indices.push_back(k2 + 1);
            }
        }
    }

    return indices;
}
