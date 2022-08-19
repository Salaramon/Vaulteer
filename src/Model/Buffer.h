#pragma once

#include <glad/glad.h>
#include <type_traits>


struct BufferType {
	inline static constexpr GLenum ArrayBuffer = GL_ARRAY_BUFFER;
	inline static constexpr GLenum ElementBuffer = GL_ELEMENT_ARRAY_BUFFER;
	inline static constexpr GLenum ShaderStorageBuffer = GL_SHADER_STORAGE_BUFFER;
	inline static constexpr GLenum QueryBuffer = GL_QUERY_BUFFER;
	inline static constexpr GLenum TextureBuffer = GL_TEXTURE_BUFFER;
	inline static constexpr GLenum UniformBuffer = GL_UNIFORM_BUFFER;
};

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
