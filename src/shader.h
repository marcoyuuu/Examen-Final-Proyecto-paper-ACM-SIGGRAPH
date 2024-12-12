// Shader.h

#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>

class Shader {
public:
    // Program ID
    GLuint ID;

    // Constructor reads and builds the shader
    Shader(const char* vertexPath, const char* fragmentPath);

    // Use/activate the shader
    void use() const;

    // Utility uniform functions
    void setBool(const std::string &name, bool value) const;  
    void setInt(const std::string &name, int value) const;   
    void setFloat(const std::string &name, float value) const;
    void setVec3(const std::string &name, const glm::vec3 &value) const;
    void setMat4(const std::string &name, const glm::mat4 &mat) const;

private:
    // Utility function for checking shader compilation/linking errors.
    void checkCompileErrors(GLuint shader, std::string type) const;
};

#endif // SHADER_H
