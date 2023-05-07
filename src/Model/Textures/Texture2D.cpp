#include "vpch.h"
#include "Texture2D.h"


Texture2D::Texture2D(TextureResourceLocator locator, bool mipmapEnabled, GLenum repeatX, GLenum repeatY) :
	Texture(mipmapEnabled),
    locator(locator) {

    createTexture(GL_TEXTURE_2D);
    setWrap(repeatX, repeatY);

	createSingleImageTexture();

    std::cout << std::format("Allocated texture from file {}: ID {}", locator.path, textureID) << std::endl;
}

Texture2D::Texture2D(int width, int height, bool mipmapEnabled, GLenum repeatX, GLenum repeatY) :
    Texture(width, height, mipmapEnabled) {

    createTexture(GL_TEXTURE_2D);
    setWrap(repeatX, repeatY);

    std::cout << std::format("Allocated texture: ID {}", textureID) << std::endl;
}

Texture2D::Texture2D(int width, int height, std::vector<glm::vec4> colors) :
    Texture(width, height) {

}

Texture2D::Texture2D(Texture2D& other) noexcept
	: Texture(other.width, other.height, other.mipmapEnabled), locator(other.locator) {
	textureID = other.textureID;
	other.textureID = 0;
}

Texture2D::Texture2D(Texture2D&& other) noexcept
	: Texture(other.width, other.height, other.mipmapEnabled), locator(std::move(other.locator)) {
	textureID = other.textureID;
	other.textureID = 0;
}

Texture2D::~Texture2D() {
	cleanup();
}


void Texture2D::setupBlankTexture(GLenum internalFormat, GLenum format) const {
	glTextureStorage2D(textureID, 1, internalFormat, width, height);
}

void Texture2D::createSingleImageTexture() {
	int loadCount = 0;

	GLsizei w, h, comp;
	byte* data = stbi_load(locator.path.data(), &w, &h, &comp, 0);

    std::cout << std::format("Loaded file gave width: {}, height: {}, component: {}", w, h, comp) << std::endl;

    auto [inFormat, exFormat] = getFormatsFromComponents(comp);
	createTextureFromData(inFormat, exFormat, data);

	if (data)
		stbi_image_free(data);
}

void Texture2D::createTextureFromData(GLenum internalFormat, GLenum format, byte* data) {
	glTextureStorage2D(textureID, 1, internalFormat, width, height);
	glTextureSubImage2D(textureID, 0, 0, 0, width, height, format, GL_UNSIGNED_BYTE, data);

	if (mipmapEnabled)
		glGenerateTextureMipmap(textureID);
}


GLint Texture2D::getTextureID() const {
	return textureID;
}

void Texture2D::setWrap(GLenum x, GLenum y) const {
    std::cout << std::format("Texture wrap set to x: {}, y: {}", x, y) << std::endl;
    glTextureParameteri(textureID, GL_TEXTURE_WRAP_S, x);
    glTextureParameteri(textureID, GL_TEXTURE_WRAP_T, y);
}
