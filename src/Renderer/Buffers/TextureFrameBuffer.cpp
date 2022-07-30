#include "vpch.h"
#include "TextureFrameBuffer.h"

TextureFrameBuffer::TextureFrameBuffer() : FrameBuffer() {}

TextureFrameBuffer::TextureFrameBuffer(uint frameBufferTexId) : frameBufferTexId(frameBufferTexId) {
	initWithTexture();
}

TextureFrameBuffer::TextureFrameBuffer(uint frameBufferTexId, uint fbo)
	: FrameBuffer(fbo), frameBufferTexId(frameBufferTexId)
{}

TextureFrameBuffer::TextureFrameBuffer(TextureFrameBuffer&& mv) noexcept
	: FrameBuffer(fbo), frameBufferTexId(mv.frameBufferTexId) {
	mv.fbo = 0;
	mv.frameBufferTexId = 0;
}

TextureFrameBuffer::~TextureFrameBuffer() {
	destroy();
}

bool TextureFrameBuffer::initWithTexture() {
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

void TextureFrameBuffer::destroy() {
	if (fbo != 0) {
		glDeleteFramebuffers(1, &fbo);
	}

	if (frameBufferTexId != 0) {
		glDeleteTextures(1, &frameBufferTexId);
	}
}

void TextureFrameBuffer::bindWrite()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
}

void TextureFrameBuffer::bindRead()
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo);
}

uint TextureFrameBuffer::getTextureId()
{
	return frameBufferTexId;
}

