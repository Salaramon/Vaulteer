#include "Texture.h"

Texture::Texture() {}

Texture::Texture(bool mipmapEnabled) : mipmapEnabled(mipmapEnabled) {}

Texture::Texture(GLsizei width, GLsizei height, bool mipmapEnabled) : width(width), height(height), mipmapEnabled(mipmapEnabled) {}


std::pair<GLint, GLint> Texture::getFormatsFromComponents(int nrComponents) {
    return [nrComponents]() {
        switch (nrComponents) {
        case STBI_rgb_alpha:	return std::make_pair(GL_RGBA8, GL_RGBA);
        case STBI_rgb:			return std::make_pair(GL_RGB8, GL_RGB);
        case STBI_grey:			return std::make_pair(GL_R8, GL_RED);
        case STBI_grey_alpha:	return std::make_pair(GL_RG8, GL_RG);
        default: throw std::runtime_error("invalid format");
        }
    }();
}

void Texture::createTexture(GLenum type) {
    glCreateTextures(type, 1, &textureID);
}

void Texture::cleanup() {
    glDeleteTextures(1, &textureID);
}

void Texture::setMinifyingFilter(GLenum filter) {
    glTextureParameteri(textureID, GL_TEXTURE_MIN_FILTER, filter);
}

void Texture::setMagnifyingFilter(GLenum filter) {
    glTextureParameteri(textureID, GL_TEXTURE_MAG_FILTER, filter);
}

void Texture::setAnisotropicFilter(GLfloat filter) {
    glTextureParameterf(textureID, GL_TEXTURE_MAX_ANISOTROPY, filter);
}
