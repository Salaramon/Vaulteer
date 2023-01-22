#include "vpch.h"
#include "Texture2DArray.h"

#include <utility>

Texture2DArray::Texture2DArray(const std::vector<TextureResourceLocator>& locators, bool mipmapEnabled, GLenum repeatX, GLenum repeatY)
	: Texture(mipmapEnabled), numLayers(locators.size()), locators(locators) {
	createTexture(GL_TEXTURE_2D_ARRAY);
	setWrap(repeatX, repeatY);
}

Texture2DArray::Texture2DArray(TextureResourceLocator locator, bool mipmapEnabled, GLenum repeatX, GLenum repeatY)
	: Texture2DArray(std::vector({std::move(locator)}), mipmapEnabled, repeatX, repeatY) {}

Texture2DArray::Texture2DArray(int width, int height, bool mipmapEnabled, GLenum repeatX, GLenum repeatY)
	: Texture(width, height, mipmapEnabled), numLayers(1) {
	createTexture(GL_TEXTURE_2D);
	setWrap(repeatX, repeatY);
}

Texture2DArray::Texture2DArray(int width, int height, const std::vector<glm::vec4>& colors)
	: Texture(width, height), numLayers(1) {
	createGeneratedTexture(colors);
}

Texture2DArray::Texture2DArray(Texture2DArray&& other) noexcept
	: Texture(other.width, other.height, other.mipmapEnabled), locators(other.locators), types(other.types), units(other.units) {
	textureID = other.textureID;
	numLayers = other.numLayers;

	other.textureID = 0;
}

Texture2DArray::~Texture2DArray() {
	cleanup();
}

GLint Texture2DArray::getTextureID() const {
	return textureID;
}

Texture2DArray::TextureUnit Texture2DArray::getUnit(const std::string& texturePath) const {
	return units.at(texturePath);
}


void Texture2DArray::createUnpacked() {
	std::vector<Image2D> images;
	GLsizei maxW = 0, maxH = 0, maxComp = 0;
	int loadCount = 0;

	for (int i = 0; i < locators.size(); i++) {
		GLsizei w, h, comp;
		byte* data = stbi_load(locators[i].path.data(), &w, &h, &comp, 0);
		images.push_back(Image2D(TextureUnit(this, 0, 0, w, h, i, locators[i].type), locators[i].path, data));
		if (images[i].loaded()) {
			loadCount++;
			maxW = std::max(maxW, width);
			maxH = std::max(maxH, height);
			maxComp = std::max(comp, maxComp);
		}
		else std::cout << "Failed to load texture: \t" + locators[i].path + "\n";
	}

	width = maxW;
	height = maxH;
	nrComponents = maxComp;

	if (loadCount == locators.size()) {
		auto [inFormat, exFormat] = getFormatsFromComponents(maxComp);
		createTextureArrayFromData(inFormat, exFormat, images);
	}
	else std::cout << "Texture data not stored: \t" + std::to_string(textureID) + "\n";

	for (Image2D& img : images) {
		if (img.loaded())
			stbi_image_free(img.data);

		units[img.path] = img.unit;
	}
}

void Texture2DArray::createTextureArrayFromData(GLenum internalFormat, GLenum format, const std::vector<Image2D>& images) const {
	glTextureStorage3D(textureID, 1, internalFormat, width, height, 1); // TODO layers
	
	for (const Image2D& img : images) {
		glTextureSubImage3D(textureID, 0, img.unit.x, img.unit.y, img.unit.layer, img.unit.w, img.unit.h, 1, format, GL_UNSIGNED_BYTE, img.data);
		std::cout << "- init texture with locator: " << img.path << std::endl;
		std::cout << "- size: " << sizeof(img.data) << std::endl;
		std::cout << "- unit: " << img.unit.x << ":" << img.unit.y << ":" << img.unit.w << ":" << img.unit.h << std::endl;
	}

	if (mipmapEnabled)
		glGenerateTextureMipmap(textureID);
}

void Texture2DArray::createGeneratedTexture(const std::vector<glm::vec4>& colors) {
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


void Texture2DArray::setWrap(GLenum x, GLenum y) const {
	glTextureParameteri(textureID, GL_TEXTURE_WRAP_S, x);
	glTextureParameteri(textureID, GL_TEXTURE_WRAP_T, y);
}
