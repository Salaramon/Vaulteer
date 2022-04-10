#include "Texture2DArray.h"

Texture2DArray::Texture2DArray(std::vector<TextureResourceLocator> locators, bool mipmapEnabled, GLenum repeatX, GLenum repeatY)
        : Texture(mipmapEnabled), numLayers(locators.size()), locators(locators){
    createTexture(GL_TEXTURE_2D_ARRAY);
    setWrap(repeatX, repeatY);

    debug("Allocated texture array from a " + std::to_string(locators.size()) + " file vector: ID " + std::to_string(textureID) + "\n");
}

Texture2DArray::Texture2DArray(TextureResourceLocator locator, bool mipmapEnabled, GLenum repeatX, GLenum repeatY)
        : Texture2DArray(std::vector({ locator }), mipmapEnabled, repeatX, repeatY) {
}

Texture2DArray::Texture2DArray(GLsizei width, GLsizei height, bool mipmapEnabled, GLenum repeatX, GLenum repeatY) 
        : Texture(width, height), numLayers(1) {
    createTexture(GL_TEXTURE_2D);
    setWrap(repeatX, repeatY);

    debug("Allocated texture array: ID " + std::to_string(textureID) + "\n");
}

Texture2DArray::Texture2DArray(GLsizei width, GLsizei height, std::vector<glm::vec4> colors) 
        : Texture(width, height), numLayers(1) {
    createGeneratedTexture(colors);
    debug("Loaded hardcoded " + std::to_string(colors.size()) + " colors texture: ID " + std::to_string(textureID) + "\n");
}

Texture2DArray::Texture2DArray(Texture2DArray&& other) noexcept {
    textureID = other.textureID;
    other.textureID = 0;
    numLayers = other.numLayers;
}

Texture2DArray::~Texture2DArray() {
    cleanup();
}

GLint Texture2DArray::getTextureID() const {
    return textureID;
}

Texture2DArray::TextureUnit Texture2DArray::getUnit(std::string texturePath) const {
    return units.at(texturePath);
}


void Texture2DArray::createUnpacked() {
    std::vector<Image2D> images;
    GLsizei maxW = 0, maxH = 0, maxComp = 0;
    int loadCount = 0;

    for (int i = 0; i < locators.size(); i++) {
        GLsizei w, h, comp;
        byte* data = stbi_load(locators[i].path.data(), &w, &h, &comp, 0);
        images.push_back(Image2D( TextureUnit(this, 0, 0, w, h, i, locators[i].type), locators[i].path, data));
        if (images[i].loaded()) {
            loadCount++;
            maxW = std::max(maxW, width);
            maxH = std::max(maxH, height);
            maxComp = std::max(comp, maxComp);
        }
        else debug("Failed to load texture: \t" + locators[i].path + "\n");
    }

    width = maxW;
    height = maxH;
    nrComponents = maxComp;

    if (loadCount == locators.size()) {
        auto [inFormat, exFormat] = getFormatsFromComponents(maxComp);
        createTextureArrayFromData(inFormat, exFormat, images);
    }
    else debug("Texture data not stored: \t" + std::to_string(textureID) + "\n");

    for (Image2D& img : images) {
        if (img.loaded())
            stbi_image_free(img.data);

        units[img.path] = img.unit;
    }
}

void Texture2DArray::createTextureArrayFromData(GLenum internalFormat, GLenum format, std::vector<Image2D> images) {
    glTextureStorage3D(textureID, 1, internalFormat, width, height, numLayers);

    for (const Image2D& img : images) {
        glTextureSubImage3D(textureID, 0, img.unit.x, img.unit.y, img.unit.layer, img.unit.w, img.unit.h, 1, format, GL_UNSIGNED_BYTE, img.data);
    }

    if (mipmapEnabled)
        glGenerateTextureMipmap(textureID);
}

void Texture2DArray::createGeneratedTexture(std::vector<glm::vec4> colors) {
    std::vector<unsigned char> pixelData;
    for (auto& color : colors) {
        pixelData.push_back(static_cast<unsigned char>(color.x * 255));
        pixelData.push_back(static_cast<unsigned char>(color.y * 255));
        pixelData.push_back(static_cast<unsigned char>(color.z * 255));
        pixelData.push_back(static_cast<unsigned char>(color.w * 255));
    }
    glCreateTextures(GL_TEXTURE_2D_ARRAY, 1, &textureID);

    glTextureStorage3D(textureID, 1, GL_RGBA8, width, height, 1);
    glTextureSubImage3D(textureID, 0, 0, 0, 0, width, height, 1, GL_RGBA, GL_UNSIGNED_BYTE, pixelData.data());

    glGenerateTextureMipmap(textureID);
}


void Texture2DArray::setWrap(GLenum x, GLenum y) {
    glTextureParameteri(textureID, GL_TEXTURE_WRAP_S, x);
    glTextureParameteri(textureID, GL_TEXTURE_WRAP_T, y);
}