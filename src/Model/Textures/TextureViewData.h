#pragma once

#include <assimp/material.h>
#include <rectpack2D/finders_interface.h>
#include <stb_image.h>

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

	// scrolling texture?
	// float scrollX;
	// float scrollY;

	TextureData() = default;
	TextureData(int textureViewId) : textureViewId(textureViewId) {}
};


// image data. container used in texture load process in Texture classes
struct Image2D {
	TextureView view;

	bool fileBacked;
	std::string path;

	int w = -1, h = -1, channels = -1;
	byte* data = nullptr;
	GLenum internalFormat, dataFormat;

	Image2D(const TextureResourceLocator& locator) : Image2D(locator.path, locator.type) {}

	Image2D(const std::string& filePath, aiTextureType type = aiTextureType_NONE) : fileBacked(true) {
		bool ok = stbi_info(filePath.data(), &w, &h, &channels);
		assert(ok);

		path = filePath;
		view = TextureView({0, 0, w, h}, 0, type);
		auto [inFormat, exFormat] = getFormatsFromChannels(channels);
		internalFormat = inFormat;
		dataFormat = exFormat;
	}
	
	Image2D(const std::vector<uint32_t>& pixels, int width, int height) : fileBacked(false), w(width), h(height) {
		channels = STBI_rgb_alpha;
		internalFormat = GL_RGBA8;
		dataFormat = GL_RGBA;
		view = TextureView({0, 0, width, height}, 0, aiTextureType_DIFFUSE);
		
		data = static_cast<byte*>(std::malloc(sizeof(uint32_t) * pixels.size()));
		std::memcpy(data, pixels.data(), sizeof(uint32_t) * pixels.size());
	}

	bool load() {
		if (!fileBacked && data)
			return true;

		data = stbi_load(path.data(), &w, &h, &channels, 0);
		return loaded();
	}

	bool free() {
		if (!data)
			return false;

		if (fileBacked)
			stbi_image_free(data);
		else
			std::free(data);
		
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
