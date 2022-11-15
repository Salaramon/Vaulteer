#include "vpch.h"
#include "GBuffer.h"


GBuffer::GBuffer(unsigned int width, unsigned int height)
		: FrameBuffer(NumTextures), width(width), height(height) {
	init();
}

bool GBuffer::init() {
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);

	// Create the gbuffer textures
 	textures[Position] = initTexture(GL_RGBA16F, GL_RGBA, GL_FLOAT);
	textures[Normal_Material] = initTexture(GL_RGBA16F, GL_RGBA, GL_FLOAT);
	textures[Color_Specular] = initTexture(GL_RGBA16F, GL_RGBA, GL_UNSIGNED_BYTE);
	depthTexture = initTexture(GL_DEPTH24_STENCIL8, GL_DEPTH_COMPONENT, GL_FLOAT);

	glDrawBuffers(NumTextures, drawBuffers.data());

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	assert(status == GL_FRAMEBUFFER_COMPLETE);

	// restore default FBO
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	return true;
}

std::shared_ptr<Texture2D> GBuffer::initTexture(GLenum internalFormat, GLenum format, GLenum type) {
	Texture2D texture = Texture2D(width, height);
	texture.setupBlankTexture(internalFormat, format);
	texture.setMinifyingFilter(GL_NEAREST);
	texture.setMagnifyingFilter(GL_NEAREST);

	GLenum attachment = (format == GL_DEPTH_COMPONENT ? GL_DEPTH_STENCIL_ATTACHMENT : GL_COLOR_ATTACHMENT0 + colorTexturesInitialized++);
	glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, texture.textureID, 0);

	return std::make_shared<Texture2D>(texture);
}

void GBuffer::bindReadTexture(GBufferTextureType textureType) const {
	glBindTexture(GL_TEXTURE_2D, textures[textureType]->textureID);
}


void GBuffer::bindTextureUnit(GBufferTextureType textureType) const {
	glBindTextureUnit(textureType, textures[textureType]->textureID);
}
