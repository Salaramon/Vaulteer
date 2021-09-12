#include "ShadowBuffer.h"

ShadowBuffer::ShadowBuffer(const uint w, const uint h) : FrameBuffer(initTexture(w, h)), 
	width(w), height(h)
{}

ShadowBuffer::ShadowBuffer(ShadowBuffer&& mv) noexcept : 
	FrameBuffer(std::move(mv)),
	width(mv.width), height(mv.height) 
{}

ShadowBuffer::~ShadowBuffer() {
	destroy();
}

uint ShadowBuffer::initTexture(uint width, uint height) {
	uint texId;
	glGenTextures(1, &texId);
	glBindTexture(GL_TEXTURE_2D, texId);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	return texId;
}

