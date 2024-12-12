// Texture.cpp

#define STBI_ENABLE_EXR               // Enable OpenEXR support
#define STB_IMAGE_IMPLEMENTATION      // Implement stb_image in this translation unit
#include <stb_image.h>
#include "Texture.h"
#include <iostream>

// Constructor for 2D textures
Texture::Texture(const std::string& path, const std::string& type)
    : id(0), type(type), path(path)
{}

// Constructor for cubemap textures
Texture::Texture(const std::vector<std::string>& paths, const std::string& type)
    : id(0), type(type), paths(paths)
{}

// Destructor
Texture::~Texture()
{
    if (id != 0) {
        glDeleteTextures(1, &id);
    }
}

// Move Constructor
Texture::Texture(Texture&& other) noexcept
    : id(other.id), type(std::move(other.type)), path(std::move(other.path)), paths(std::move(other.paths))
{
    other.id = 0; // Prevent the moved-from object from deleting the texture
}

// Move Assignment Operator
Texture& Texture::operator=(Texture&& other) noexcept
{
    if (this != &other)
    {
        // Delete existing texture if any
        if (id != 0)
        {
            glDeleteTextures(1, &id);
        }

        // Transfer ownership
        id = other.id;
        type = std::move(other.type);
        path = std::move(other.path);
        paths = std::move(other.paths);

        // Reset the source texture ID
        other.id = 0;
    }
    return *this;
}

// Loads 2D texture
bool Texture::load()
{
    glGenTextures(1, &id);
    int width, height, nrComponents;
    bool isHDR = false;

    // Determine if the texture is HDR based on its type
    if (type == "texture_normal" || type == "texture_roughness")
    {
        isHDR = true;
        stbi_set_flip_vertically_on_load(true); // Flip normals and roughness maps if necessary
    }
    else
    {
        stbi_set_flip_vertically_on_load(false);
    }

    if (isHDR)
    {
        float* data = stbi_loadf(path.c_str(), &width, &height, &nrComponents, 0);
        if (data)
        {
            GLenum format;
            if (nrComponents == 1)
                format = GL_RED;
            else if (nrComponents == 3)
                format = GL_RGB;
            else if (nrComponents == 4)
                format = GL_RGBA;
            else
                format = GL_RGB; // Default

            glBindTexture(GL_TEXTURE_2D, id);
            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_FLOAT, data);
            glGenerateMipmap(GL_TEXTURE_2D);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            stbi_image_free(data);
            std::cout << "Loaded HDR texture: " << path << " with ID: " << id << std::endl;
            return true;
        }
        else
        {
            std::cerr << "HDR Texture failed to load at path: " << path << ": " << stbi_failure_reason() << std::endl;
            stbi_image_free(data);
            return false;
        }
    }
    else
    {
        unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrComponents, 0);
        if (data)
        {
            GLenum format;
            if(nrComponents == 1)
                format = GL_RED;
            else if(nrComponents == 3)
                format = GL_RGB;
            else if(nrComponents == 4)
                format = GL_RGBA;
            else
                format = GL_RGB; // Default
            
            glBindTexture(GL_TEXTURE_2D, id);
            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
            stbi_image_free(data);
            std::cout << "Loaded texture: " << path << " with ID: " << id << std::endl;
            return true;
        }
        else
        {
            std::cerr << "Texture failed to load at path: " << path << ": " << stbi_failure_reason() << std::endl;
            stbi_image_free(data);
            return false;
        }
    }
}

// Loads cubemap texture
bool Texture::loadCubemap()
{
    if(paths.empty())
    {
        std::cerr << "No paths provided for cubemap texture." << std::endl;
        return false;
    }

    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_CUBE_MAP, id);

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(false); // Skyboxes typically don't need flipping
    for(unsigned int i = 0; i < paths.size(); ++i)
    {
        unsigned char *data = stbi_load(paths[i].c_str(), &width, &height, &nrChannels, 0);
        if(data)
        {
            GLenum format;
            if(nrChannels == 1)
                format = GL_RED;
            else if(nrChannels == 3)
                format = GL_RGB;
            else if(nrChannels == 4)
                format = GL_RGBA;
            else
                format = GL_RGB; // Default fallback

            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 
                         0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data
            );
            stbi_image_free(data);
        }
        else
        {
            std::cerr << "Cubemap texture failed to load at path: " 
                      << paths[i] << std::endl;
            stbi_image_free(data);
            return false; // Optionally, return false or continue loading other faces
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Using GL_CLAMP_TO_EDGE to prevent seams
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);  
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);  
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);  

    // Generate mipmaps for the skybox
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

    std::cout << "Loaded cubemap texture with ID: " << id << std::endl;
    return true;
}
