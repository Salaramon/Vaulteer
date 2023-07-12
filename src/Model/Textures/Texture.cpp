#include "vpch.h"
#include "Model/Textures/Texture.h"

#include "API/Log.h"

void Texture::createTexture(GLenum type) {
    glCreateTextures(type, 1, &textureID);
    Log::trace("Created texture of type '{}' with id '{}'", type, textureID);
}

void Texture::cleanup() const {
    glDeleteTextures(1, &textureID);
    Log::trace("Deleted texture with id '{}'", textureID);
}

void Texture::setMinifyingFilter(GLenum filter) const {
    glTextureParameteri(textureID, GL_TEXTURE_MIN_FILTER, filter);
}

void Texture::setMagnifyingFilter(GLenum filter) const {
    glTextureParameteri(textureID, GL_TEXTURE_MAG_FILTER, filter);
}

void Texture::setAnisotropicFilter(GLfloat filter) const {
    glTextureParameterf(textureID, GL_TEXTURE_MAX_ANISOTROPY, filter);
}
