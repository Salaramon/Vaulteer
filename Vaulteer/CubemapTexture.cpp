#include "CubemapTexture.h"

CubemapTexture::CubemapTexture(std::array<std::string, 6> const& paths, bool mipmapEnabled,
                               GLenum repeatX, GLenum repeatY, GLenum repeatZ) {
    debug("Loading cubemap: " + paths[0] + "\n");

    createTexture(GL_TEXTURE_CUBE_MAP);
    createTextureFromFiles(paths);
}

CubemapTexture::CubemapTexture(CubemapTexture&& other) noexcept {
    textureID = other.textureID;
    other.textureID = 0;
}

CubemapTexture::~CubemapTexture() {
    cleanup();
}

GLint CubemapTexture::getTextureID() const {
    return textureID;
}

void CubemapTexture::createTextureFromFiles(std::array<std::string, 6> const& paths) {
    int comp;
    std::array<byte*, 6> faces;
    std::array<bool, 6> loaded;
    int loadCount = 0;

    for (int i = 0; i < 6; i++) {
        faces[i] = stbi_load(paths[i].data(), &width, &height, &comp, 0);
        if (faces[i]) {
            loaded[i] = true;
            loadCount++;
        }
        else debug("Failed to load texture: \n\t" + *paths.data() + "\n");
    }

    if (loadCount == 6) {
        auto [inFormat, exFormat] = getFormatsFromComponents(comp);
        nrComponents = comp;
        createCubeTexture(inFormat, exFormat, faces);
    }

    for (int i = 0; i < 6; i++) {
        if (loaded[i])
            stbi_image_free(faces[i]);
    }
}

void CubemapTexture::createCubeTexture(GLenum internalFormat, GLenum format, std::array<byte*, 6> data) {
    glTextureStorage2D(textureID, 1, internalFormat, width, height);

    for (GLint i = 0; i < 6; ++i) {
        glTextureSubImage3D(textureID, 0, 0, 0, i, width, height, 1, format, GL_UNSIGNED_BYTE, data[i]);
    }

    glGenerateTextureMipmap(textureID);
}

void CubemapTexture::setWrap(GLenum x, GLenum y, GLenum z) {
    glTextureParameteri(textureID, GL_TEXTURE_WRAP_S, x);
    glTextureParameteri(textureID, GL_TEXTURE_WRAP_T, y);
    glTextureParameteri(textureID, GL_TEXTURE_WRAP_R, z);
}
