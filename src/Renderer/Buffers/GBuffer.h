#pragma once

#include <glad/glad.h>
#include <iostream>

#include "Framebuffer.h"
#include "Model/Textures/Texture2D.h"

class GBuffer : public Framebuffer {
public:
    enum GBufferTextureType {
        Position,               // vec3 position
        Normal_Material,        // vec3 normal   + int material_index
        Color_Specular,         // vec3 diffuse  + float specular
        NumTextures
    };

    GBuffer(unsigned int width, unsigned int height);

    bool init();

    void bindReadTexture(GBufferTextureType textureType) const;
    void bindTextureUnit(GBufferTextureType textureType) const;

    void copyDepth(GLint destFBO) {
        bindForReading();
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, destFBO);
		glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT, GL_NEAREST);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    }
    
    unsigned int width, height;

private:
    unsigned int colorTexturesInitialized = 0;
    std::array<GLenum, 3> drawBuffers = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };

    std::array<std::shared_ptr<Texture2D>, NumTextures> textures;
    std::shared_ptr<Texture2D> depthTexture;

    std::shared_ptr<Texture2D> initTexture(GLenum internalFormat, GLenum format, GLenum type);
};

