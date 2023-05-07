#pragma once

#include "Texture.h"
#include "TextureResourceLocator.h"
#include "TextureViewData.h"

class Texture2DArray : public Texture {
public:
	Texture2DArray(const std::vector<TextureResourceLocator>& locators, bool mipmapEnabled = true, GLenum repeatX = GL_CLAMP_TO_EDGE, GLenum repeatY = GL_CLAMP_TO_EDGE);
	Texture2DArray(TextureResourceLocator locator, bool mipmapEnabled = true, GLenum repeatX = GL_CLAMP_TO_EDGE, GLenum repeatY = GL_CLAMP_TO_EDGE);
	Texture2DArray(int width, int height, bool mipmapEnabled = true, GLenum repeatX = GL_CLAMP_TO_EDGE, GLenum repeatY = GL_CLAMP_TO_EDGE);

	Texture2DArray(int width, int height, const std::vector<glm::vec4>& colors);

	template <class... Args>
		requires (std::conjunction_v<std::is_same<glm::vec4, Args>...>)
	Texture2DArray(GLsizei width, GLsizei height, glm::vec4 first, Args ...args);

	Texture2DArray(Texture2DArray&& other) noexcept;
	~Texture2DArray();

	GLint getTextureID() const;

	Texture2DArray() {
		createTexture(GL_TEXTURE_2D_ARRAY);
	}
protected:

	void createUnpacked();

	void createTextureArrayFromData(GLenum internalFormat, GLenum format, const std::vector<Image2D>& images) const;

	void setWrap(GLenum x, GLenum y) const;

	void createGeneratedTexture(const std::vector<glm::vec4>& colors);

	GLsizei numLayers = 1;

	std::vector<TextureResourceLocator> locators;
	std::vector<aiTextureType> types;
};

template <class ...Args>
	requires (std::conjunction_v<std::is_same<glm::vec4, Args>...>)
Texture2DArray::Texture2DArray(GLsizei width, GLsizei height, glm::vec4 first, Args ...args) : Texture(width, height) {
	createGeneratedTexture({first, args...});
};
