#include "Shader.h"
#include <fstream>
#include <sstream>
#include <iostream>

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();		
        vShaderFile.close();
        fShaderFile.close();
        vertexCode   = vShaderStream.str();
        fragmentCode = fShaderStream.str();		
    } catch(std::ifstream::failure& e) {
        std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }

    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    GLuint vertex, fragment;
    GLint success;
    GLchar infoLog[1024];

    // Vertex Shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX");

    // Fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");

    // Shader Program
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    // Check link errors
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(ID, 1024, NULL, infoLog);
        std::cerr << "ERROR::PROGRAM_LINKING_ERROR\n" << infoLog << std::endl;
        // If link failed, set ID=0
        glDeleteProgram(ID);
        ID = 0;
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::use() const
{
    if(ID != 0) glUseProgram(ID);
    else {
        // If invalid program, print a warning (only once)
        static bool warned = false;
        if(!warned) {
            std::cerr << "WARNING::SHADER::use() called on invalid shader program." << std::endl;
            warned = true;
        }
    }
}

void Shader::setBool(const std::string &name, bool value) const
{
    if(ID == 0) return; // Skip if invalid
    GLint loc = glGetUniformLocation(ID, name.c_str());
    if(loc >= 0) glUniform1i(loc, (int)value);
}

void Shader::setInt(const std::string &name, int value) const
{
    if(ID == 0) return; // Skip if invalid
    GLint loc = glGetUniformLocation(ID, name.c_str());
    if(loc >= 0) glUniform1i(loc, value);
}

void Shader::setFloat(const std::string &name, float value) const
{
    if(ID == 0) return;
    GLint loc = glGetUniformLocation(ID, name.c_str());
    if(loc >= 0) glUniform1f(loc, value);
}

void Shader::setVec3(const std::string &name, const glm::vec3 &value) const
{
    if(ID == 0) return;
    GLint loc = glGetUniformLocation(ID, name.c_str());
    if(loc >= 0) glUniform3fv(loc, 1, &value[0]);
}

void Shader::setMat4(const std::string &name, const glm::mat4 &mat) const
{
    if(ID == 0) return;
    GLint loc = glGetUniformLocation(ID, name.c_str());
    if(loc >= 0) glUniformMatrix4fv(loc, 1, GL_FALSE, &mat[0][0]);
}

void Shader::checkCompileErrors(GLuint shader, std::string type) const
{
    GLint success;
    GLchar infoLog[1024];
    if(type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if(!success) {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cerr << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" 
                      << infoLog << "\n -- --------------------------------------------------- -- " 
                      << std::endl;
        }
    }
    else
    {
        // Linking handled outside
    }
}
