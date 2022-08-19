#include "vpch.h"
#include "AlphaBuffer.h"


AlphaBuffer::AlphaBuffer(unsigned int width, unsigned int height)
	: FrameBuffer(NumTextures), width(width), height(height) {
	init();
}

bool AlphaBuffer::init() {
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);

	// Create the AlphaBuffer textures
	textures[Accumulated] = initTexture(GL_RGBA16F, GL_RGBA, GL_FLOAT);
	textures[Alpha] = initTexture(GL_R8, GL_RED, GL_FLOAT);
	depthTexture = initTexture(GL_DEPTH24_STENCIL8, GL_DEPTH_COMPONENT, GL_FLOAT);

	glDrawBuffers(NumTextures, drawBuffers);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	assert(status == GL_FRAMEBUFFER_COMPLETE);

	// restore default FBO
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	return true;
}

void AlphaBuffer::clearColor() {
	glClearNamedFramebufferfv(fbo, GL_COLOR, 0, &glm::vec4(0.0)[0]);
	glClearNamedFramebufferfv(fbo, GL_COLOR, 1, &glm::vec1(1.0)[0]);
}

std::shared_ptr<Texture2D> AlphaBuffer::initTexture(GLenum internalFormat, GLenum format, GLenum type) {
	Texture2D texture = Texture2D(width, height);
	texture.createBlankTexture(internalFormat, format);
	texture.setMinifyingFilter(GL_NEAREST);
	texture.setMagnifyingFilter(GL_NEAREST);

	GLenum attachment = (format == GL_DEPTH_COMPONENT ? GL_DEPTH_STENCIL_ATTACHMENT : GL_COLOR_ATTACHMENT0 + colorTexturesInitialized++);
	glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, texture.textureID, 0);

	return std::make_shared<Texture2D>(texture);
}

void AlphaBuffer::bindReadTexture(AlphaBufferTextureType textureType) const {
	glBindTexture(GL_TEXTURE_2D, textures[textureType]->textureID);
}


void AlphaBuffer::bindTextureUnit(AlphaBufferTextureType textureType) const {
	glBindTextureUnit(textureType, textures[textureType]->textureID);
}
