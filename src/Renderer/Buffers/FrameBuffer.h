#pragma once

class FrameBuffer {
public:
	FrameBuffer() {
		glCreateFramebuffers(1, &fbo);
	}

	FrameBuffer(GLuint fbo) {
		this->fbo = fbo;
	}

	FrameBuffer(FrameBuffer&& other) noexcept : fbo(other.fbo) {
		other.fbo = 0;
	}

	~FrameBuffer() {
		glDeleteBuffers(1, &fbo);
	}


	void bindForWriting() {
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
	}

	void bindForReading() {
		glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo);
	}

	void unbind() {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	GLuint fbo = 0;
};