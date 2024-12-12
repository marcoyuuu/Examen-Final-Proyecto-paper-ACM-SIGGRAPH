// Mesh.cpp

#include "Mesh.h"
#include <glad/glad.h>
#include <iostream>

// Constructor
Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, std::vector<Texture>&& textures)
    : VAO(0), VBO(0), EBO(0), indexCount(static_cast<GLsizei>(indices.size())), textures(std::move(textures))
{
    setupMesh(vertices, indices);
}

// Move constructor
Mesh::Mesh(Mesh&& other) noexcept
    : VAO(other.VAO), VBO(other.VBO), EBO(other.EBO), indexCount(other.indexCount), textures(std::move(other.textures))
{
    other.VAO = 0;
    other.VBO = 0;
    other.EBO = 0;
    other.indexCount = 0;
}

// Move assignment operator
Mesh& Mesh::operator=(Mesh&& other) noexcept
{
    if(this != &other)
    {
        // Delete existing resources
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);

        // Transfer ownership
        VAO = other.VAO;
        VBO = other.VBO;
        EBO = other.EBO;
        indexCount = other.indexCount;
        textures = std::move(other.textures);

        // Reset other's resources
        other.VAO = 0;
        other.VBO = 0;
        other.EBO = 0;
        other.indexCount = 0;
    }
    return *this;
}

// Destructor
Mesh::~Mesh()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

// Initialize buffers
void Mesh::setupMesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices)
{
    // Generate buffers and arrays
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    // Load vertex data
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    // Load index data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // Vertex Positions
    glEnableVertexAttribArray(0);	
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

    // Vertex Normals
    glEnableVertexAttribArray(1);	
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

    // Vertex Colors
    glEnableVertexAttribArray(2);	
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Color));

    // Texture Coordinates
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

    glBindVertexArray(0);
}

// Render the mesh
void Mesh::Draw(const Shader& shader) const
{
    // Bind appropriate textures
    unsigned int diffuseNr  = 1;
    unsigned int normalNr   = 1;
    unsigned int roughnessNr = 1;
    for(unsigned int i = 0; i < textures.size(); ++i)
    {
        glActiveTexture(GL_TEXTURE0 + i); // Activate proper texture unit before binding

        // Retrieve texture number (e.g., diffuse1, normal1)
        std::string number;
        std::string name = textures[i].getType();
        if(name == "texture_diffuse")
            number = std::to_string(diffuseNr++);
        else if(name == "texture_normal")
            number = std::to_string(normalNr++);
        else if(name == "texture_roughness")
            number = std::to_string(roughnessNr++);
        else
            number = "1"; // Default number

        // Now set the sampler to the correct texture unit
        shader.setInt((name + number).c_str(), i);
        // Bind the texture
        glBindTexture(GL_TEXTURE_2D, textures[i].getID());
    }

    // Draw mesh
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);

    // Reset active texture
    glActiveTexture(GL_TEXTURE0);
}
