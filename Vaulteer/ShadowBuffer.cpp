#include "ShadowBuffer.h"

ShadowBuffer::ShadowBuffer(const unsigned int width, const unsigned int height) :
	width(width), height(height)
{
	init();
}

ShadowBuffer::ShadowBuffer(ShadowBuffer&& mv) : width(mv.width), height(mv.height) {
	this->FBO = mv.FBO;
	this->shadowMapTexId = mv.shadowMapTexId;
	mv.FBO = 0;
	mv.shadowMapTexId = 0;
}

ShadowBuffer::~ShadowBuffer()
{
	if (FBO != 0) {
		glDeleteFramebuffers(1, &FBO);
	}

	if (shadowMapTexId != 0) {
		glDeleteTextures(1, &shadowMapTexId);
	}
}

bool ShadowBuffer::init()
{
	glGenFramebuffers(1, &FBO);

	glGenTextures(1, &shadowMapTexId);
	glBindTexture(GL_TEXTURE_2D, shadowMapTexId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMapTexId, 0);

	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE) {
		printf("FB error, status: 0x%x\n", status);
		return false;
	}

	// Restore default framebuffer after init
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	return true;
}

void ShadowBuffer::bindWrite()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, FBO);
}

void ShadowBuffer::bindRead()
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, FBO);
}
