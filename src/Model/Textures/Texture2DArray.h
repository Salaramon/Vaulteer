#pragma once
#include <rectpack2D/finders_interface.h>

#include "Texture.h"
#include "TextureResourceLocator.h"

#include "Utils/TypeDefUtils.h"

class Texture2DArray : public Texture {
protected:
	// disallow flipping of textures in packing
	using spaces_type = rectpack2D::empty_spaces<false, rectpack2D::default_empty_spaces>;
	using rect_type = rectpack2D::output_rect_t<spaces_type>;

public:
	/// <summary>
	/// Defining bounds of a subtexture in a texture collection. Includes texture type for filtering.
	/// </summary>
	struct TextureUnit : rect_type {
		Texture2DArray* texture;

		int layer;
		aiTextureType type;

		TextureUnit() = default;

		TextureUnit(Texture2DArray* texture) :
			rect_type(0, 0, 0, 0),
			texture(texture),
			layer(0),
			type(aiTextureType_NONE) {}

		TextureUnit(const TextureUnit& unit, rect_type rect) :
			rect_type(rect),
			texture(unit.texture),
			layer(unit.layer),
			type(unit.type) {}

		TextureUnit(Texture2DArray* texture, const int x, const int y, const int w, const int h, const int layer, aiTextureType type) :
			rect_type(x, y, w, h),
			texture(texture),
			layer(layer),
			type(type) {}

		TextureUnit minus(const TextureUnit& other) {
			return {
				texture,
				x - other.x,
				y - other.y,
				w - other.w,
				h - other.h,
				layer - other.layer,
				type
			};
		}
	};

	struct Image2D {
		TextureUnit unit;
		std::string path;
		byte* data;

		bool loaded() const {
			return data;
		}
	};

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

	TextureUnit getUnit(const std::string& texturePath) const;

protected:
	Texture2DArray() {}

	void createUnpacked();

	void createTextureArrayFromData(GLenum internalFormat, GLenum format, const std::vector<Image2D>& images) const;

	void setWrap(GLenum x, GLenum y) const;

	void createGeneratedTexture(const std::vector<glm::vec4>& colors);

	GLsizei numLayers;

	std::vector<TextureResourceLocator> locators;
	std::vector<aiTextureType> types;

	// units indexed by texture path
	std::unordered_map<std::string, TextureUnit> units;
};

template <class ...Args>
	requires (std::conjunction_v<std::is_same<glm::vec4, Args>...>)
Texture2DArray::Texture2DArray(GLsizei width, GLsizei height, glm::vec4 first, Args ...args) : width(width), height(height) {
	createGeneratedTexture({first, args...});
};
