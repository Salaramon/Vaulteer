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
    void clearColor() override;

    void bindReadTexture(AlphaBufferTextureType TextureType) const;
    void bindTextureUnit(AlphaBufferTextureType textureType) const;

private:
    unsigned int width, height;
    unsigned int colorTexturesInitialized = 0;
    GLenum drawBuffers[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };

    std::array<std::shared_ptr<Texture2D>, NumTextures> textures;
    std::shared_ptr<Texture2D> depthTexture;

    std::shared_ptr<Texture2D> initTexture(GLenum internalFormat, GLenum format, GLenum type);
};

