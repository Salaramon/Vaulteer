#pragma once

#include "Texture.h"
#include "TextureResourceLocator.h"
#include "TextureViewData.h"
#include "Utils/TypeDefUtils.h"

class Texture2D : public Texture {
public:
	Texture2D(int width, int height, GLenum internalFormat, bool mipmapEnabled = true, GLenum repeatX = GL_CLAMP_TO_EDGE, GLenum repeatY = GL_CLAMP_TO_EDGE)
			: Texture(width, height, mipmapEnabled) {

		createTexture(GL_TEXTURE_2D);
		reserve(internalFormat);

		glTextureParameteri(textureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(textureID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTextureParameteri(textureID, GL_TEXTURE_WRAP_S, repeatX);
		glTextureParameteri(textureID, GL_TEXTURE_WRAP_T, repeatY);
	}

	Texture2D(Image2D& img, bool mipmapEnabled = true, GLenum repeatX = GL_CLAMP_TO_EDGE, GLenum repeatY = GL_CLAMP_TO_EDGE)
			: Texture(img.w, img.h, mipmapEnabled) {

		createTexture(GL_TEXTURE_2D);
		initialize(img);

		glTextureParameteri(textureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(textureID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTextureParameteri(textureID, GL_TEXTURE_WRAP_S, repeatX);
		glTextureParameteri(textureID, GL_TEXTURE_WRAP_T, repeatY);
	}

	Texture2D(Texture2D& other) noexcept
			: Texture(other.w, other.h, other.mipmapEnabled) {
		textureID = other.textureID;
		other.textureID = 0;
	}

	Texture2D(Texture2D&& other) noexcept
			: Texture(other.w, other.h, other.mipmapEnabled) {
		textureID = other.textureID;
		other.textureID = 0;
	}

	~Texture2D() {
		cleanup();
	}

	void reserve(GLenum internalFormat) {
		glTextureStorage2D(textureID, 1, internalFormat, w, h);
	}

	void initialize(Image2D& img) {
		reserve(img.internalFormat);

		assert(img.load());

		glTextureSubImage2D(textureID, 0, 0, 0, w, h, img.dataFormat, GL_UNSIGNED_BYTE, img.data);

		if (mipmapEnabled)
			glGenerateTextureMipmap(textureID);

		assert(img.free());
	}

	GLint getTextureID() const {
		return textureID;
	}
};
