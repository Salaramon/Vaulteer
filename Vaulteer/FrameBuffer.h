#pragma once

#include <glad/glad.h>
#include <iostream>

#include "TypeDefUtils.h"

/*
	Abstract frame buffer. Subclasses implement initTexture() with textures that populate the frame buffer when bound
*/
class FrameBuffer {
public:
	FrameBuffer();
	FrameBuffer(uint frameBufferTexId);
	FrameBuffer(uint frameBufferTexId, uint FBO);
	FrameBuffer(FrameBuffer&& mv) noexcept;

	~FrameBuffer();

	void destroy();

	virtual uint initTexture(uint width, uint height) = 0;
	bool initFramebuffer();

	void bindWrite();
	void bindRead();

	uint getTextureId();

private:
	uint frameBufferTexId;
	uint FBO;
};
