#include "GBuffer.h"


GBuffer::GBuffer(unsigned int windowWidth, unsigned int windowHeight)
    : windowWidth(windowWidth), windowHeight(windowHeight)
{
    init();
}

GBuffer::~GBuffer()
{
    glDeleteFramebuffers(1, &FBO);
}

bool GBuffer::init()
{
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, FBO);

    // Create the gbuffer textures
    glGenTextures(NumTextures, textures);
    glGenTextures(1, &depthTexture);

    initTexture(textures[Position], GL_RGBA16F, GL_RGBA, GL_FLOAT);
    initTexture(textures[Normal], GL_RGBA16F, GL_RGBA, GL_FLOAT);
    initTexture(textures[Color], GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE); // TODO intention is to combine color and specular data into single texture here - specular(last byte) currently not implemented
    initTexture(depthTexture, GL_DEPTH_COMPONENT24, GL_DEPTH_COMPONENT, GL_FLOAT);

    GLenum DrawBuffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
    glDrawBuffers(NumTextures, DrawBuffers);

    GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        
    if (Status != GL_FRAMEBUFFER_COMPLETE) {
        printf("FB error, status: 0x%x\n", Status);
        return false;
    }

    // restore default FBO
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

    return true;
}

void GBuffer::initTexture(GLuint texture, GLenum internalFormat, GLenum format, GLenum type)
{
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, windowWidth, windowHeight, 0, format, type, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    GLenum attachment = (format == GL_DEPTH_COMPONENT ? GL_DEPTH_ATTACHMENT : GL_COLOR_ATTACHMENT0 + colorTexturesInitialized++);
    glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, texture, 0);
}

void GBuffer::bindForWriting()
{
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, FBO);
}

void GBuffer::bindForReading()
{
    glBindFramebuffer(GL_READ_FRAMEBUFFER, FBO);
}

void GBuffer::setReadBuffer(GBufferTextureType TextureType)
{
    glBindTexture(GL_TEXTURE_2D, textures[TextureType]);
}
