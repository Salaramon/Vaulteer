#pragma once
#include "Texture.h"

class Texture2DArray : protected Texture {
public:
    Texture2DArray(std::vector<std::string> paths, bool mipmapEnabled = true, glm::vec2 offset = glm::vec2(), GLenum repeatX = GL_CLAMP_TO_EDGE, GLenum repeatY = GL_CLAMP_TO_EDGE);
    Texture2DArray(std::string path, bool mipmapEnabled = true, glm::vec2 offset = glm::vec2(), GLenum repeatX = GL_CLAMP_TO_EDGE, GLenum repeatY = GL_CLAMP_TO_EDGE);
    Texture2DArray(GLsizei width, GLsizei height, bool mipmapEnabled = true, glm::vec2 offset = glm::vec2(), GLenum repeatX = GL_CLAMP_TO_EDGE, GLenum repeatY = GL_CLAMP_TO_EDGE);
    
    Texture2DArray(GLsizei width, GLsizei height, std::vector<glm::vec4> colors);

    template<class... Args>
    requires (std::conjunction_v<std::is_same<glm::vec4, Args>...>)
    Texture2DArray(GLsizei width, GLsizei height, glm::vec4 first, Args ...args);

    Texture2DArray(Texture2DArray&& other) noexcept;
    ~Texture2DArray();

    GLint getTextureID() const;

protected:
    void createTextureArrayFromFiles(std::vector<std::string> paths);
    void createTextureArray(GLenum internalFormat, GLenum format, std::vector<byte*> data);

    void setWrap(GLenum x, GLenum y);

    void createGeneratedTexture(std::vector<glm::vec4> colors);

    std::vector<aiTextureType> textureTypes;
    GLsizei numLayers;
};

template<class ...Args>
requires (std::conjunction_v<std::is_same<glm::vec4, Args>...>)
inline Texture2DArray::Texture2DArray(GLsizei width, GLsizei height, glm::vec4 first, Args ...args) : width(width), height(height) {
    createGeneratedTexture({ first, args... });
};
