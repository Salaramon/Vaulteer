#pragma once

class FrameBuffer {
public:
	FrameBuffer(int numTextures) : numTextures(numTextures) {
		glCreateFramebuffers(1, &fbo);
	}

	FrameBuffer(GLuint fbo, int numTextures) : fbo(fbo), numTextures(numTextures) 
	{}

	FrameBuffer(FrameBuffer&& other) noexcept : fbo(other.fbo), numTextures(other.numTextures){
		other.fbo = 0;
	}

	virtual ~FrameBuffer() {
		glDeleteBuffers(1, &fbo);
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