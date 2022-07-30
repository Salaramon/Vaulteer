#pragma once

#include <glad/glad.h>
#include <iostream>

#include "FrameBuffer.h"
#include "Model/Textures/Texture2D.h"

class AlphaBuffer : public FrameBuffer
{
public:
    enum AlphaBufferTextureType {
        Accumulated,
        Alpha,
        NumTextures
    };

    AlphaBuffer(unsigned int width, unsigned int height);

    bool init();

    void bindReadTexture(AlphaBufferTextureType TextureType);
    void bindTextureUnit(AlphaBufferTextureType textureType);

private:
    unsigned int width, height;
    unsigned int colorTexturesInitialized = 0;

    std::array<std::shared_ptr<Texture2D>, NumTextures> textures;
    std::shared_ptr<Texture2D> depthTexture;

    std::shared_ptr<Texture2D> initTexture(GLenum internalFormat, GLenum format, GLenum type);
};

