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
	int textureViewId = -1;
	rect_type rect;
	int layer;
	aiTextureType type;

	// Uniform buffer representation
	struct TextureViewData {
		int xDelta, yDelta, wDelta, hDelta, layerDelta;
		TextureViewData() : xDelta(0), yDelta(0), wDelta(0), hDelta(0), layerDelta(1) {}
		TextureViewData(const TextureView& u) : xDelta(u.rect.x), yDelta(u.rect.y), wDelta(u.rect.w), hDelta(u.rect.h), layerDelta(u.layer) {}
	};

	TextureView() :
		rect(0, 0, 0, 0),
		layer(0),
		type(aiTextureType_NONE) {}

	TextureView(const TextureView& unit, rect_type rect) :
		rect(rect),
		layer(unit.layer),
		type(unit.type) {}

	TextureView(rect_type rect, int layer, aiTextureType type) :
		rect(rect),
		layer(layer),
		type(type) {}
	
	TextureView minus(const TextureView& other) {
		return {
			{rect.x - other.rect.x,
			rect.y - other.rect.y,
			rect.w - other.rect.w,
			rect.h - other.rect.h},
			layer - other.layer,
			type
		};
	}
};

// Uniform buffer representation
struct TextureData {
	int textureViewId = 0;
	int viewFlags = 0;

	// scrolling texture?
	// float scrollX;
	// float scrollY;

	TextureData() = default;
	TextureData(int textureViewId) : textureViewId(textureViewId) {}

	void putView(const TextureView& view) {
		switch (view.type) {
		case aiTextureType_DIFFUSE: viewFlags |= 1 << 0;
			break;
		case aiTextureType_SPECULAR: viewFlags |= 1 << 1;
			break;
		case aiTextureType_HEIGHT: viewFlags |= 1 << 2;
			break;
		default: ;
		}
	}
};


// file backed image data
class Image2D {
public:
	TextureView view;
	std::string path;
	int width = -1, height = -1, channels = -1;
	byte* data = nullptr;

	Image2D(const TextureResourceLocator& locator) {
		bool error = stbi_info(locator.path.data(), &width, &height, &channels);
		assert(!error);

		path = locator.path;
		view = TextureView({0, 0, width, height}, 0, locator.type);
	}

	bool load() {
		data = stbi_load(path.data(), &width, &height, &channels, 0);
		return loaded();
	}

	bool loaded() const {
		return data;
	}
};
