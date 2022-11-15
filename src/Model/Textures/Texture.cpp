#include "vpch.h"
#include "Model/Textures/Texture.h"

Texture::Texture() : 
    OR(this,
        DY::V(&textureID, &width, &height, &nrComponents, &mipmapEnabled, &uniformTextureTypes),
        DY::N("textureID", "width", "height", "nrComponents", "mipmapEnabled", "uniformTextureTypes")) {
    OB.add(OR);
}

Texture::Texture(bool mipmapEnabled) :
    OR(this,
        DY::V(&textureID, &width, &height, &nrComponents, &this->mipmapEnabled, &uniformTextureTypes),
        DY::N("textureID", "width", "height", "nrComponents", "mipmapEnabled", "uniformTextureTypes")),
    
    mipmapEnabled(mipmapEnabled) {
    OB.add(OR);
}

Texture::Texture(GLsizei width, GLsizei height, bool mipmapEnabled) :
    OR(this,
        DY::V(&textureID, &this->width, &this->height, &nrComponents, &this->mipmapEnabled, &uniformTextureTypes),
        DY::N("textureID", "width", "height", "nrComponents", "mipmapEnabled", "uniformTextureTypes")),

    width(width), height(height), mipmapEnabled(mipmapEnabled) {
    OB.add(OR);
}


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
    LOG::CLAS::debug<&Texture::createTexture>(this, &textureID, DY::std_format("Created texture of type '{}' with id '{}'", type, textureID));
}

void Texture::cleanup() {
    glDeleteTextures(1, &textureID);
    LOG::CLAS::debug<&Texture::cleanup>(this, &textureID, DY::std_format("Deleted texture with id '{}'", textureID));
}

void Texture::setMinifyingFilter(GLenum filter) {
    glTextureParameteri(textureID, GL_TEXTURE_MIN_FILTER, filter);
    LOG::CLAS::debug<&Texture::setMinifyingFilter>(this, &textureID, DY::std_format("Min filter of texture with id '{}' set to '{}'", textureID, filter));
}

void Texture::setMagnifyingFilter(GLenum filter) {
    glTextureParameteri(textureID, GL_TEXTURE_MAG_FILTER, filter);
    LOG::CLAS::debug<&Texture::setMagnifyingFilter>(this, &textureID, DY::std_format("Mag filter of texture with id '{}' set to '{}'", textureID, filter));
}

void Texture::setAnisotropicFilter(GLfloat filter) {
    glTextureParameterf(textureID, GL_TEXTURE_MAX_ANISOTROPY, filter);
    LOG::CLAS::debug<&Texture::setAnisotropicFilter>(this, &textureID, DY::std_format("Anisotrophy filter of texture with id '{}' set to '{}'", textureID, filter));
}
