#pragma once

#include <glad/glad.h>
#include <iostream>

class GBuffer
{
public:

    enum GBufferTextureType {
        Position,
        Normal,
        Color,
        NumTextures
    };


    GBuffer(unsigned int windowWidth, unsigned int windowHeight);

    ~GBuffer();

    bool init();

    void bindForWriting();

    void bindForReading();

    void setReadBuffer(GBufferTextureType TextureType);


    GLuint textures[NumTextures];

private:
    unsigned int windowWidth, windowHeight;
    unsigned int colorTexturesInitialized = 0;

    GLuint FBO;//, RBO;
    GLuint depthTexture;

    void initTexture(GLuint texture, GLenum internalFormat, GLenum format, GLenum type);
};

