#pragma once

#include "API/Core.h"
#include "Framebuffer.h"
#include "Utils/TypeDefUtils.h"

/*
	Abstract frame buffer. Subclasses should implement initTexture() with textures that populate the frame buffer when bound
*/
class TextureFrameBuffer {
public:
	Framebuffer framebuffer;

	TextureFrameBuffer(FramebufferSpecification spec) : framebuffer(spec) {}
	TextureFrameBuffer(uint frameBufferTexId, FramebufferSpecification spec) : framebuffer(spec), frameBufferTexId(frameBufferTexId) {
		KYSE_ASSERT(initWithTexture());
	}

	TextureFrameBuffer(TextureFrameBuffer&& mv) noexcept : framebuffer(std::move(mv.framebuffer)), frameBufferTexId(mv.frameBufferTexId) {}

	~TextureFrameBuffer();

	void destroy() const;

	virtual uint initTexture(uint width, uint height) = 0;

	bool initWithTexture() const;

	void bindWrite() const;
	void bindRead() const;

	uint getTextureId() const;

protected:
	uint frameBufferTexId;
};
