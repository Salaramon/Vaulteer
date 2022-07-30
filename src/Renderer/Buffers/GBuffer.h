#pragma once

#include <glad/glad.h>
#include <iostream>

#include "FrameBuffer.h"
#include "Model/Textures/Texture2D.h"

class GBuffer : public FrameBuffer
{
public:

    enum GBufferTextureType {
        Position,               // vec3 position
        Normal_Material,        // vec3 normal + int material_index
        Color_Specular,         // vec3 diffuse + float specular
        NumTextures
    };

    GBuffer(unsigned int width, unsigned int height);

    bool init();

    void bindReadTexture(GBufferTextureType TextureType);
    void bindTextureUnit(GBufferTextureType textureType);

    unsigned int width, height;

private:
    unsigned int colorTexturesInitialized = 0;

    std::array<std::shared_ptr<Texture2D>, NumTextures> textures;
    std::shared_ptr<Texture2D> depthTexture;

    std::shared_ptr<Texture2D> initTexture(GLenum internalFormat, GLenum format, GLenum type);
};

