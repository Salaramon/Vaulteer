#pragma once

#include <glad/glad.h>
#include <iostream>

#include "FrameBuffer.h"
#include "Utils/TypeDefUtils.h"

/*
	Abstract frame buffer. Subclasses should implement initTexture() with textures that populate the frame buffer when bound
*/
class TextureFrameBuffer : FrameBuffer {
public:
	TextureFrameBuffer();
	TextureFrameBuffer(uint frameBufferTexId);
	TextureFrameBuffer(uint frameBufferTexId, uint fbo);
	TextureFrameBuffer(TextureFrameBuffer&& mv) noexcept;

	~TextureFrameBuffer();

	void destroy();

	virtual uint initTexture(uint width, uint height) = 0;

	bool initWithTexture();

	void bindWrite();
	void bindRead();

	uint getTextureId();

protected:
	uint frameBufferTexId;
};
