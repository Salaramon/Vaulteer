#include "ShadowMapFBO.h"

ShadowMapFBO::ShadowMapFBO()
{
}

ShadowMapFBO::~ShadowMapFBO()
{
	if (FBO != 0) {
		glDeleteFramebuffers(1, &FBO);
	}

	if (shadowMapTexId != 0) {
		glDeleteTextures(1, &shadowMapTexId);
	}
}

bool ShadowMapFBO::init(unsigned int width, unsigned int height)
{
	glGenFramebuffers(1, &FBO);

	glGenTextures(1, &shadowMapTexId);
	glBindTexture(GL_TEXTURE_2D, shadowMapTexId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMapTexId, 0);

	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE) {
		printf("FB error, status: 0x%x\n", status);
		return false;
	}
	return true;
}

void ShadowMapFBO::bindWrite()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, FBO);
}

void ShadowMapFBO::bindRead(GLenum TextureUnit)
{
	glActiveTexture(TextureUnit);
	glBindTexture(GL_TEXTURE_2D, shadowMapTexId);
}