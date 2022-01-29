#include "Texture2DArray.h"

Texture2DArray::Texture2DArray(std::vector<std::string> paths, bool mipmapEnabled, glm::vec2 offset, GLenum repeatX, GLenum repeatY) : Texture(mipmapEnabled, offset), numLayers(paths.size()) {
    createTexture(GL_TEXTURE_2D_ARRAY);
    createTextureArrayFromFiles(paths);
    setWrap(repeatX, repeatY);

    debug("Allocated texture array from a " + std::to_string(paths.size()) + " file vector: ID " + std::to_string(textureID));
}

Texture2DArray::Texture2DArray(std::string path, bool mipmapEnabled, glm::vec2 offset, GLenum repeatX, GLenum repeatY) : 
    Texture2DArray(std::vector({ path }), mipmapEnabled, offset, repeatX, repeatY) {
}

Texture2DArray::Texture2DArray(GLsizei width, GLsizei height, bool mipmapEnabled, glm::vec2 offset, GLenum repeatX, GLenum repeatY) : Texture(width, height), numLayers(1) {
    createTexture(GL_TEXTURE_2D_ARRAY);
    setWrap(repeatX, repeatY);

    debug("Allocated texture array: ID " + std::to_string(textureID));
}

Texture2DArray::Texture2DArray(GLsizei width, GLsizei height, std::vector<glm::vec4> colors) : Texture(width, height), numLayers(1) {
    createGeneratedTexture(colors);
    debug("Loaded hardcoded " + std::to_string(colors.size()) + " colors texture: ID " + std::to_string(textureID));
}

Texture2DArray::Texture2DArray(Texture2DArray&& other) noexcept : Texture(other) {
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



void Texture2DArray::createTextureArrayFromFiles(std::vector<std::string> paths) {
    int comp;
    std::vector<byte*> data;
    std::vector<bool> loaded;
    GLsizei arrayW = 0, arrayH = 0;
    int loadCount = 0;

    for (int i = 0; i < paths.size(); i++) {
        data.push_back(stbi_load(paths[i].data(), &width, &height, &comp, 0));
        if (data[i]) {
            loaded.push_back(true);
            loadCount++;
            arrayW = std::max(arrayW, width);
            arrayH = std::max(arrayH, height);
        }
        else debug("Failed to load texture: \n\t" + *paths.data() + "\n");
    }
    width = arrayW;
    height = arrayH;

    if (loadCount == paths.size()) {
        auto [inFormat, exFormat] = getFormatsFromComponents(comp);
        nrComponents = comp;
        createTextureArray(inFormat, exFormat, data);
    }
    else debug("Texture not created: \n\t" + *paths.data() + "\n");

    for (int i = 0; i < paths.size(); i++) {
        if (loaded[i])
            stbi_image_free(data[i]);
    }
}

void Texture2DArray::createTextureArray(GLenum internalFormat, GLenum format, std::vector<byte*> data) {
    glTextureStorage3D(textureID, 4, internalFormat, width, height, numLayers);

    for (GLint layer = 0; layer < data.size(); layer++) {
        glTextureSubImage3D(textureID, 0, offset.x, offset.y, layer, width, height, 1, format, GL_UNSIGNED_BYTE, data[layer]);
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