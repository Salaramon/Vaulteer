#include "vpch.h"
#include "Model/Textures/PackedTexture2DArray.h"

PackedTexture2DArray::PackedTexture2DArray(const std::vector<TextureResourceLocator>& locators, bool mipmapEnabled, GLenum repeatX, GLenum repeatY)
	: Texture2DArray(locators, mipmapEnabled, repeatX, repeatY) {
	for (const TextureResourceLocator& loc : locators)
		std::cout << "init texture with locator: " << loc.path << std::endl;

	//glTextureParameteri(textureID, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // hack OMG!!!!!
	//glTextureParameteri(textureID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

PackedTexture2DArray::PackedTexture2DArray(TextureResourceLocator locator, bool mipmapEnabled, GLenum repeatX, GLenum repeatY)
	: Texture2DArray(std::move(locator), mipmapEnabled, repeatX, repeatY) {}

PackedTexture2DArray::PackedTexture2DArray(GLsizei width, GLsizei height, bool mipmapEnabled, GLenum repeatX, GLenum repeatY)
	: Texture2DArray(width, height, mipmapEnabled, repeatX, repeatY) {}

