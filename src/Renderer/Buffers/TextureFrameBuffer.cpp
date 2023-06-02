#include "vpch.h"
#include "TextureFrameBuffer.h"

TextureFrameBuffer::TextureFrameBuffer() : Framebuffer(1) {}

TextureFrameBuffer::TextureFrameBuffer(uint frameBufferTexId) : Framebuffer(1), frameBufferTexId(frameBufferTexId) {
	initWithTexture();
}

TextureFrameBuffer::TextureFrameBuffer(uint frameBufferTexId, uint fbo)
	: Framebuffer(fbo, 1), frameBufferTexId(frameBufferTexId)
{}

TextureFrameBuffer::TextureFrameBuffer(TextureFrameBuffer&& mv) noexcept
	: Framebuffer(fbo, 1), frameBufferTexId(mv.frameBufferTexId) {
	mv.fbo = 0;
	mv.frameBufferTexId = 0;
}

TextureFrameBuffer::~TextureFrameBuffer() {
	destroy();
}

bool TextureFrameBuffer::initWithTexture() const {
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, frameBufferTexId, 0);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE) {
		printf("FB error, status: 0x%x\n", status);
		return false;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	return true;
}

void TextureFrameBuffer::destroy() const {
	if (fbo != 0) {
		glDeleteFramebuffers(1, &fbo);
	}

	if (frameBufferTexId != 0) {
		glDeleteTextures(1, &frameBufferTexId);
	}
}

void TextureFrameBuffer::bindWrite() const {
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
}

void TextureFrameBuffer::bindRead() const {
	glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo);
}

uint TextureFrameBuffer::getTextureId() const {
	return frameBufferTexId;
}

