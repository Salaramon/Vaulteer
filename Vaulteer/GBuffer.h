#pragma once

#include <glad/glad.h>
#include <iostream>

class GBuffer
{
public:

    enum GBUFFER_TEXTURE_TYPE {
        GBUFFER_TEXTURE_TYPE_POSITION,
        GBUFFER_TEXTURE_TYPE_DIFFUSE,
        GBUFFER_TEXTURE_TYPE_NORMAL,
        GBUFFER_TEXTURE_TYPE_TEXCOORD,
        GBUFFER_NUM_TEXTURES
    };

    GBuffer();

    ~GBuffer();

    bool Init(unsigned int WindowWidth, unsigned int WindowHeight);

    void BindForWriting();

    void BindForReading();

    void SetReadBuffer(GBuffer::GBUFFER_TEXTURE_TYPE TextureType);

private:
	GLuint FBO;
    GLuint textures[GBUFFER_NUM_TEXTURES];
    GLuint depthTexture;

};

