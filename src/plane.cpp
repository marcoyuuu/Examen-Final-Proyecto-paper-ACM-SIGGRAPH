// Plane.cpp

#include "Plane.h"
#include <glad/glad.h>
#include "stb_image.h"
#include <iostream>

// Constructor
Plane::Plane()
{
    // Initialize planeMesh with no textures; textures will be loaded in Setup()
}

// Destructor
Plane::~Plane() = default;

// Setup method
void Plane::Setup()
{
    // Load textures
    textures.clear(); // Ensure no residual textures

    // Load Diffuse Texture with Absolute Path
    Texture diffuseTexture("C:/Users/yumar/OneDrive/Desktop/COMP 4046/OpenGL/main7/assets/textures/plane/coast_sand_rocks_02_diff_2k.jpg", "texture_diffuse");
    if (!diffuseTexture.load())
    {
        std::cerr << "Failed to load plane diffuse texture." << std::endl;
    }
    textures.push_back(std::move(diffuseTexture));

    // Load Normal Map with Absolute Path
    Texture normalTexture("C:/Users/yumar/OneDrive/Desktop/COMP 4046/OpenGL/main7/assets/textures/plane/coast_sand_rocks_02_nor_gl_2k.exr", "texture_normal");
    if (!normalTexture.load())
    {
        std::cerr << "Failed to load plane normal texture." << std::endl;
    }
    textures.push_back(std::move(normalTexture));

    // Load Roughness Map with Absolute Path
    Texture roughnessTexture("C:/Users/yumar/OneDrive/Desktop/COMP 4046/OpenGL/main7/assets/textures/plane/coast_sand_rocks_02_rough_2k.exr", "texture_roughness");
    if (!roughnessTexture.load())
    {
        std::cerr << "Failed to load plane roughness texture." << std::endl;
    }
    textures.push_back(std::move(roughnessTexture));

    // Generate vertices and indices
    std::vector<Vertex> planeVertices = generatePlaneVertices();
    std::vector<unsigned int> planeIndices = generatePlaneIndices();

    // Initialize the Mesh with vertices, indices, and textures
    planeMesh = std::make_unique<Mesh>(planeVertices, planeIndices, std::move(textures));
}

// Generates vertices for a large ground plane
std::vector<Vertex> Plane::generatePlaneVertices() const
{
    std::vector<Vertex> vertices(4);

    // Define four corners of the plane
    vertices[0].Position = glm::vec3(-50.0f, 0.0f, -50.0f);
    vertices[0].Normal = glm::vec3(0.0f, 1.0f, 0.0f);
    vertices[0].Color = glm::vec3(0.3f, 0.5f, 0.3f); // Greenish
    vertices[0].TexCoords = glm::vec2(0.0f, 0.0f);

    vertices[1].Position = glm::vec3(50.0f, 0.0f, -50.0f);
    vertices[1].Normal = glm::vec3(0.0f, 1.0f, 0.0f);
    vertices[1].Color = glm::vec3(0.3f, 0.5f, 0.3f); // Greenish
    vertices[1].TexCoords = glm::vec2(50.0f, 0.0f);

    vertices[2].Position = glm::vec3(50.0f, 0.0f, 50.0f);
    vertices[2].Normal = glm::vec3(0.0f, 1.0f, 0.0f);
    vertices[2].Color = glm::vec3(0.3f, 0.5f, 0.3f); // Greenish
    vertices[2].TexCoords = glm::vec2(50.0f, 50.0f);

    vertices[3].Position = glm::vec3(-50.0f, 0.0f, 50.0f);
    vertices[3].Normal = glm::vec3(0.0f, 1.0f, 0.0f);
    vertices[3].Color = glm::vec3(0.3f, 0.5f, 0.3f); // Greenish
    vertices[3].TexCoords = glm::vec2(0.0f, 50.0f);

    return vertices;
}

// Generates indices for a quad ground plane
std::vector<unsigned int> Plane::generatePlaneIndices() const
{
    return {
        0, 1, 2,
        2, 3, 0
    };
}

// Renders the ground plane
void Plane::draw(const Shader &shader) const
{
    if (!planeMesh)
    {
        std::cerr << "Plane mesh not initialized. Call Setup() before drawing." << std::endl;
        return;
    }

    // Disable backface culling to render both sides of the plane
    glDisable(GL_CULL_FACE);
    
    // Set model matrix
    glm::mat4 model = glm::mat4(1.0f);
    shader.setMat4("model", model);

    // Draw the mesh
    planeMesh->Draw(shader);
    
    // Re-enable backface culling for other objects
    glEnable(GL_CULL_FACE);
}
