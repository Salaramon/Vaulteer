#include "vpch.h"
#include "TextureFrameBuffer.h"


bool TextureFrameBuffer::initWithTexture() const {
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer.fbo);
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
	if (framebuffer.fbo != 0) {
		glDeleteFramebuffers(1, &framebuffer.fbo);
	}

	if (frameBufferTexId != 0) {
		glDeleteTextures(1, &frameBufferTexId);
	}
}

void TextureFrameBuffer::bindWrite() const {
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, framebuffer.fbo);
}

void TextureFrameBuffer::bindRead() const {
	glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuffer.fbo);
}

uint TextureFrameBuffer::getTextureId() const {
	return frameBufferTexId;
}

