#pragma once

#include "Framebuffer.h"
#include "Utils/TypeDefUtils.h"

/*
	Abstract frame buffer. Subclasses should implement initTexture() with textures that populate the frame buffer when bound
*/
class TextureFrameBuffer : Framebuffer {
public:
	TextureFrameBuffer();
	TextureFrameBuffer(uint frameBufferTexId);
	TextureFrameBuffer(uint frameBufferTexId, uint fbo);
	TextureFrameBuffer(TextureFrameBuffer&& mv) noexcept;

	~TextureFrameBuffer() override;

	void destroy() const;

	virtual uint initTexture(uint width, uint height) = 0;

	bool initWithTexture() const;

	void bindWrite() const;
	void bindRead() const;

	uint getTextureId() const;

protected:
	uint frameBufferTexId;
};
