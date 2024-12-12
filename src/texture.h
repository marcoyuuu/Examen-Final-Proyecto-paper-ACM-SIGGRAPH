// Texture.h

#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <vector>
#include <glad/glad.h> // Include glad before any OpenGL headers

class Texture {
public:
    // Constructor for 2D textures
    Texture(const std::string& path, const std::string& type);

    // Constructor for cubemap textures
    Texture(const std::vector<std::string>& paths, const std::string& type);

    // Destructor
    ~Texture();

    // Delete copy constructor and copy assignment to prevent copying
    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;

    // Implement move constructor and move assignment for efficient transfers
    Texture(Texture&& other) noexcept;
    Texture& operator=(Texture&& other) noexcept;

    // Loads the texture from the file(s)
    bool load();
    bool loadCubemap();

    // Getters
    GLuint getID() const { return id; }
    const std::string& getType() const { return type; }
    const std::string& getPath() const { return path; }

private:
    GLuint id;             // OpenGL texture ID
    std::string type;     // Texture type (e.g., diffuse, normal, cubemap)
    std::string path;     // File path to the texture (for 2D textures)
    std::vector<std::string> paths; // File paths for cubemap textures
};

#endif // TEXTURE_H
