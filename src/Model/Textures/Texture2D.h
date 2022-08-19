#pragma once

#include "Texture.h"
#include "TextureResourceLocator.h"
#include "Utils/TypeDefUtils.h"

class Texture2D : public Texture {
public:
    Texture2D(TextureResourceLocator path, bool mipmapEnabled = true, GLenum repeatX = GL_CLAMP_TO_EDGE, GLenum repeatY = GL_CLAMP_TO_EDGE);
    Texture2D(int width, int height, bool mipmapEnabled = true, GLenum repeatX = GL_CLAMP_TO_EDGE, GLenum repeatY = GL_CLAMP_TO_EDGE);

    Texture2D(int width, int height, std::vector<glm::vec4> colors);

    Texture2D(Texture2D& other) noexcept;

    Texture2D(Texture2D&& other) noexcept;
    ~Texture2D();

    void createBlankTexture(GLenum internalFormat, GLenum format) const;

    GLint getTextureID() const;

    void setWrap(GLenum x, GLenum y) const;

protected:
    TextureResourceLocator locator;

    void createSingleImageTexture();
    void createTextureFromData(GLenum internalFormat, GLenum format, byte* data);
};

