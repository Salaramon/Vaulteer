#pragma once




class Framebuffer {
public:
	Framebuffer(int numTextures) : numTextures(numTextures) {
		glCreateFramebuffers(1, &fbo);
	}

	Framebuffer(GLuint fbo, int numTextures) : fbo(fbo), numTextures(numTextures) 
	{}

	Framebuffer(Framebuffer&& other) noexcept : fbo(other.fbo), numTextures(other.numTextures){
		other.fbo = 0;
	}

	virtual ~Framebuffer() {
		// 12 hours were wasted here by copypasting glDeleteBuffers from Buffer.h
		glDeleteFramebuffers(1, &fbo);
	}


	void bindForWriting() const {
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
	}

	void bindForReading() const {
		glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo);
	}

	void unbind() {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	virtual void clear() {
		clearColor();
		clearDepthStencil();
	}

	virtual void clearColor() {
		for (int i = 0; i < numTextures; i++)
			glClearNamedFramebufferfv(fbo, GL_COLOR, i, &glm::vec4(0.0f)[0]);
	}

	virtual void clearDepthStencil() {
		glClearNamedFramebufferfi(fbo, GL_DEPTH_STENCIL, 0, 1.0, 0);
	}

	GLuint fbo = 0;
	GLuint numTextures;
};