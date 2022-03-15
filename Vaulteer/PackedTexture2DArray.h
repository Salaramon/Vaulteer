#pragma once
#include "Texture2DArray.h"

class PackedTexture2DArray : public Texture2DArray {

    const size_t pack_max_texture_side_size = 8192;
    const int pack_discard_step = 1;

    const bool allow_flip = true;

public:
    PackedTexture2DArray(std::vector<TextureResourceLocator> paths, bool mipmapEnabled = true, GLenum repeatX = GL_CLAMP_TO_EDGE, GLenum repeatY = GL_CLAMP_TO_EDGE);
    PackedTexture2DArray(TextureResourceLocator path, bool mipmapEnabled = true, GLenum repeatX = GL_CLAMP_TO_EDGE, GLenum repeatY = GL_CLAMP_TO_EDGE);
    PackedTexture2DArray(GLsizei width, GLsizei height, bool mipmapEnabled = true, GLenum repeatX = GL_CLAMP_TO_EDGE, GLenum repeatY = GL_CLAMP_TO_EDGE);

    TextureUnit getUnit(std::string texturePath) const;

protected:
    void createPacked();
};

