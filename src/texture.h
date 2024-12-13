#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <vector>
#include <glad/glad.h>

/**
 * @class Texture
 * @brief Representa una textura 2D o un cubemap en OpenGL.
 *
 * La clase @c Texture se encarga de cargar una textura desde disco usando stb_image,
 * subirla a OpenGL, generar mipmaps y configurar parámetros de filtrado y wrapping.
 */
class Texture {
public:
    /**
     * @brief Constructor para texturas 2D.
     * @param path Ruta al archivo de imagen.
     * @param type Tipo de textura (ej: "texture_diffuse", "texture_normal").
     */
    Texture(const std::string& path, const std::string& type);

    /**
     * @brief Constructor para cubemaps.
     * @param paths Vector con rutas a las 6 texturas del cubemap.
     * @param type Tipo de textura (ej: "texture_cubemap").
     */
    Texture(const std::vector<std::string>& paths, const std::string& type);

    /**
     * @brief Destructor. Elimina la textura de OpenGL si existe.
     */
    ~Texture();

    // Delete copy constructor and assignment to prevent copying
    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;

    // Move semantics
    Texture(Texture&& other) noexcept;
    Texture& operator=(Texture&& other) noexcept;

    /**
     * @brief Carga una textura 2D desde @ref path en OpenGL.
     * @return true si la carga fue exitosa, false en caso contrario.
     */
    bool load();

    /**
     * @brief Carga un cubemap desde las rutas en @ref paths.
     * @return true si la carga fue exitosa, false en caso contrario.
     */
    bool loadCubemap();

    /**
     * @brief Obtiene el ID de la textura OpenGL.
     * @return GLuint ID de la textura.
     */
    GLuint getID() const { return id; }

    /**
     * @brief Obtiene el tipo de la textura.
     * @return std::string Tipo (ej: "texture_diffuse").
     */
    const std::string& getType() const { return type; }

    /**
     * @brief Obtiene la ruta del archivo si es textura 2D.
     * @return std::string Ruta del archivo.
     */
    const std::string& getPath() const { return path; }

private:
    GLuint id;                  /**< ID de la textura en OpenGL */
    std::string type;           /**< Tipo de textura (difusa, normal, roughness, cubemap, etc.) */
    std::string path;           /**< Ruta al archivo de imagen (para texturas 2D) */
    std::vector<std::string> paths; /**< Rutas de las 6 caras para cubemap */
};

#endif // TEXTURE_H
