#pragma once
#include "Model/Textures/Texture2DArray.h"

class PackedTexture2DArray : public Texture2DArray {

	const int pack_max_texture_side_size = 8192;
	const int pack_discard_step = 1;

	const bool allow_flip = true;

	bool packingErrorReported = false;

public:
	PackedTexture2DArray(const std::vector<TextureResourceLocator>& locators, bool mipmapEnabled = true, GLenum repeatX = GL_CLAMP_TO_EDGE, GLenum repeatY = GL_CLAMP_TO_EDGE);
	PackedTexture2DArray(TextureResourceLocator locator, bool mipmapEnabled = true, GLenum repeatX = GL_CLAMP_TO_EDGE, GLenum repeatY = GL_CLAMP_TO_EDGE);
	PackedTexture2DArray(GLsizei width, GLsizei height, bool mipmapEnabled = true, GLenum repeatX = GL_CLAMP_TO_EDGE, GLenum repeatY = GL_CLAMP_TO_EDGE);
};
