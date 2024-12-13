#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>

/**
 * @class Shader
 * @brief Maneja la carga, compilación y uso de shaders en OpenGL.
 *
 * La clase @c Shader encapsula la lógica necesaria para leer shaders desde archivos,
 * compilarlos y establecer sus variables uniformes.
 */
class Shader {
public:
    GLuint ID; /**< ID del programa shader. */

    /**
     * @brief Constructor que carga y compila shaders desde archivos.
     *
     * @param vertexPath Ruta al archivo del shader de vértices.
     * @param fragmentPath Ruta al archivo del shader de fragmentos.
     */
    Shader(const char* vertexPath, const char* fragmentPath);

    /**
     * @brief Activa el programa shader.
     */
    void use() const;

    /// Funciones para establecer uniformes.
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
    void setVec3(const std::string &name, const glm::vec3 &value) const;
    void setMat4(const std::string &name, const glm::mat4 &mat) const;

private:
    /**
     * @brief Comprueba errores de compilación o enlace del shader.
     *
     * @param shader ID del shader o programa.
     * @param type Tipo de operación (compilación o enlace).
     */
    void checkCompileErrors(GLuint shader, std::string type) const;
};

#endif // SHADER_H
