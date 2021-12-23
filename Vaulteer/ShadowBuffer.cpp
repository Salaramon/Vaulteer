#include "ShadowBuffer.h"

ShadowBuffer::ShadowBuffer(const uint w, const uint h) : FrameBuffer(initTexture(w, h)), 
	width(w), height(h)
{}

ShadowBuffer::ShadowBuffer(const uint size) : FrameBuffer(initTexture(size, size)), width(size), height(size) 
{}

ShadowBuffer::ShadowBuffer(const GLSLSpotLight& spotLight) {
	int size = GLSLSpotLight::calculateSizeAcross(spotLight);
	size = 512; // TODO
	width = size; 
	height = size;
	frameBufferTexId = initTexture(width, height);
 	initFramebuffer();
}

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

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	return texId;
}

