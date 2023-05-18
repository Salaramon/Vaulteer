#pragma once

#include <assimp/material.h>
#include <rectpack2D/finders_interface.h>

#include "OpenGL.h"

#include "Model/Textures/TextureResourceLocator.h"
#include "Utils/TypeDefUtils.h"

constexpr bool allow_flip = false;
using spaces_type = rectpack2D::empty_spaces<allow_flip, rectpack2D::default_empty_spaces>;
using rect_type = rectpack2D::output_rect_t<spaces_type>;

/// <summary>
/// Defining bounds of a subtexture in a texture collection. Includes texture type for filtering.
/// </summary>
struct TextureView {
	int textureViewId;
	rect_type rect;
	int layer;
	aiTextureType type;

	TextureView() :
		rect(0, 0, 0, 0),
		layer(0),
		type(aiTextureType_NONE) {}

	TextureView(rect_type rect, int layer, aiTextureType type) :
		textureViewId(-1),
		rect(rect),
		layer(layer),
		type(type) {}

	TextureView(const TextureView& view, rect_type rect) :
		textureViewId(view.textureViewId),
		rect(rect),
		layer(view.layer),
		type(view.type) {}
};

// TextureView uniform buffer representation
struct TextureViewData {
	int xDelta, yDelta, wDelta, hDelta, layerDelta;
	TextureViewData() : xDelta(0), yDelta(0), wDelta(0), hDelta(0), layerDelta(1) {}
	TextureViewData(const TextureView& u) : xDelta(u.rect.x), yDelta(u.rect.y), wDelta(u.rect.w), hDelta(u.rect.h), layerDelta(u.layer) {}
};

// Texture uniform buffer representation
struct TextureData {
	int textureViewId = 0;
	float pad = 0.5;

	// scrolling texture?
	// float scrollX;
	// float scrollY;

	TextureData() = default;
	TextureData(int textureViewId) : textureViewId(textureViewId) {}
};


// file backed image data. container used in texture load process in TextureLibrary
class Image2D {
public:
	TextureView view;
	std::string path;

	int width = -1, height = -1, channels = -1;
	byte* data = nullptr;
	GLenum internalFormat, dataFormat;

	Image2D(const TextureResourceLocator& locator) {
		bool ok = stbi_info(locator.path.data(), &width, &height, &channels);
		assert(ok);

		path = locator.path;
		view = TextureView({0, 0, width, height}, 0, locator.type);
		auto [inFormat, exFormat] = getFormatsFromChannels(channels);
		internalFormat = inFormat;
		dataFormat = exFormat;
	}

	bool load() {
		data = stbi_load(path.data(), &width, &height, &channels, 0);
		return loaded();
	}

	bool free() {
		if (!data)
			return false;

		stbi_image_free(data);
		return true;
	}

	bool loaded() const {
		return data;
	}

	// internals
	static std::pair<GLenum, GLenum> getFormatsFromChannels(int nrComponents) {
	    switch (nrComponents) {
        case STBI_rgb:			return std::make_pair(GL_RGB8, GL_RGB);
        case STBI_grey_alpha:	return std::make_pair(GL_RG8, GL_RG);
        case STBI_grey:			return std::make_pair(GL_R8, GL_RED);
        default: assert(false); // "invalid format"
		case STBI_rgb_alpha:	return std::make_pair(GL_RGBA8, GL_RGBA);
	    }
	}
};
