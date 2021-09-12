#include "FrameBuffer.h"

FrameBuffer::FrameBuffer(uint frameBufferTexId) : frameBufferTexId(frameBufferTexId) {
	initFramebuffer();
}

FrameBuffer::FrameBuffer(uint frameBufferTexId, uint FBO)
	: frameBufferTexId(frameBufferTexId), FBO(FBO)
{}

FrameBuffer::FrameBuffer(FrameBuffer&& mv) noexcept
	: frameBufferTexId(mv.frameBufferTexId), FBO(mv.FBO) {
	mv.FBO = 0;
	mv.frameBufferTexId = 0;
}

FrameBuffer::~FrameBuffer() {
	destroy();
}

bool FrameBuffer::initFramebuffer() {
	glGenFramebuffers(1, &FBO);

	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, frameBufferTexId, 0);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE) {
		printf("FB error, status: 0x%x\n", status);
		return false;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	return true;
}

void FrameBuffer::destroy() {
	if (FBO != 0) {
		glDeleteFramebuffers(1, &FBO);
	}

	if (frameBufferTexId != 0) {
		glDeleteTextures(1, &frameBufferTexId);
	}
}

void FrameBuffer::bindWrite()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, FBO);
}

void FrameBuffer::bindRead()
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, FBO);
}

uint FrameBuffer::getTextureId()
{
	return frameBufferTexId;
}

