#pragma once

#include <glad/glad.h>

template<GLenum BufferType>
class Buffer {
public:
	Buffer() {
		initialize(BufferType);
	}


	Buffer(Buffer&& other) noexcept : buffer(other.buffer) {
		other.buffer = 0;
	}

	~Buffer() {
		cleanup();
	}

	void initialize(GLuint target) {
		glCreateBuffers(1, &buffer);
		glBindBuffer(target, buffer);
	}

	void cleanup() {
		glDeleteBuffers(1, &buffer);
		buffer = 0;
	}

protected:
	GLuint buffer = 0;
};

using ArrayBuffer = Buffer<GL_ARRAY_BUFFER>;
using ElementArrayBuffer = Buffer<GL_ELEMENT_ARRAY_BUFFER>;
using ShaderStorageBuffer = Buffer<GL_SHADER_STORAGE_BUFFER>;
using QueryBuffer = Buffer<GL_QUERY_BUFFER>;
using TextureBuffer = Buffer<GL_TEXTURE_BUFFER>;
using InternalUniformBuffer = Buffer<GL_UNIFORM_BUFFER>;
