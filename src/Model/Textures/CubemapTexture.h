#pragma once
#include "Model/Textures/Texture.h"

#include "Utils/TypeDefUtils.h"

class CubemapTexture : protected Texture {
public:
    CubemapTexture(std::array<std::string, 6> const& paths, bool mipmapEnabled = true, 
                   GLenum repeatX = GL_CLAMP_TO_EDGE, GLenum repeatY = GL_CLAMP_TO_EDGE, GLenum repeatZ = GL_CLAMP_TO_EDGE);

    CubemapTexture(CubemapTexture&& other) noexcept;
    ~CubemapTexture();

    GLint getTextureID() const;

protected:
    void createTextureFromFiles(std::array<std::string, 6> const& paths);
    void createCubeTexture(GLenum internalFormat, GLenum format, std::array<byte*, 6> data);

    void setWrap(GLenum x, GLenum y, GLenum z) const;

};