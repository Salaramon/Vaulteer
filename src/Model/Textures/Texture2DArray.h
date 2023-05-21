#pragma once

#include "Texture.h"
#include "TextureResourceLocator.h"
#include "TextureViewData.h"

class Texture2DArray : public Texture {
public:
	GLsizei numLayers = 1;

	std::vector<TextureResourceLocator> locators;
	std::vector<aiTextureType> types;
	
	Texture2DArray(int width, int height, int layers = 1, bool mipmapEnabled = true, GLenum repeatX = GL_CLAMP_TO_EDGE, GLenum repeatY = GL_CLAMP_TO_EDGE)
			: Texture(width, height, mipmapEnabled), numLayers(layers) {

		createTexture(GL_TEXTURE_2D_ARRAY);
		setWrap(repeatX, repeatY);
		
		glTextureParameteri(textureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(textureID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}

	Texture2DArray(Texture2DArray&& other) noexcept
			: Texture(other.width, other.height, other.mipmapEnabled), locators(other.locators), types(other.types) {
		textureID = other.textureID;
		numLayers = other.numLayers;

		other.textureID = 0;
	}

	void initialize(std::vector<Image2D>& images) const {
		// make sure we use the largest channel type
		int maxChannels = 0;
		for (Image2D& img : images) {
			maxChannels = std::max(maxChannels, img.channels);
		}

		GLenum inFormat = Image2D::getFormatsFromChannels(maxChannels).first;
		glTextureStorage3D(textureID, 1, inFormat, width, height, numLayers);

		for (Image2D& img : images) {
			assert(img.load());

			auto& rect = img.view.rect;
			glTextureSubImage3D(textureID, 0, rect.x, rect.y, img.view.layer, rect.w, rect.h, 1, img.dataFormat, GL_UNSIGNED_BYTE, img.data);
			std::cout << "- init texture with locator: " << img.path << std::endl;
			std::cout << "- size: " << sizeof(img.data) << std::endl;
			std::cout << "- unit: " << rect.x << ":" << rect.y << ":" << rect.w << ":" << rect.h <<  " :: layer: " << img.view.layer << std::endl;
			
			assert(img.free());
		}
		
		if (mipmapEnabled)
			glGenerateTextureMipmap(textureID);
	}

	~Texture2DArray() {
		cleanup();
	}

	GLint getTextureID() const {
		return textureID;
	}

protected:
	void setWrap(GLenum x, GLenum y) const {
		glTextureParameteri(textureID, GL_TEXTURE_WRAP_S, x);
		glTextureParameteri(textureID, GL_TEXTURE_WRAP_T, y);
	}
};
