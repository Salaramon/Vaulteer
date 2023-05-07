#include "vpch.h"
#include "Model/Textures/Texture.h"


void Texture::createTexture(GLenum type) {
    glCreateTextures(type, 1, &textureID);
    std::cout << std::format("Created texture of type '{}' with id '{}'", type, textureID) << std::endl;
}

void Texture::cleanup() const {
    glDeleteTextures(1, &textureID);
    std::cout << std::format("Deleted texture with id '{}'", textureID) << std::endl;
}

void Texture::setMinifyingFilter(GLenum filter) const {
    glTextureParameteri(textureID, GL_TEXTURE_MIN_FILTER, filter);
    std::cout << std::format("Min filter of texture with id '{}' set to '{}'", textureID, filter) << std::endl;
}

void Texture::setMagnifyingFilter(GLenum filter) const {
    glTextureParameteri(textureID, GL_TEXTURE_MAG_FILTER, filter);
    std::cout << std::format("Mag filter of texture with id '{}' set to '{}'", textureID, filter) << std::endl;
}

void Texture::setAnisotropicFilter(GLfloat filter) const {
    glTextureParameterf(textureID, GL_TEXTURE_MAX_ANISOTROPY, filter);
    std::cout << std::format("Anisotrophy filter of texture with id '{}' set to '{}'", textureID, filter) << std::endl;
}
