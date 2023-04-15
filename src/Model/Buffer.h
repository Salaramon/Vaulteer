#pragma once

#include <glad/glad.h>
#include <type_traits>

#include "Debug/Debug.h"

template<GLenum BufferType>
class Buffer {
public:
	Buffer() : 
		OR(this, DY::V(&buffer), DY::N("buffer")) {
		OB.add(OR);
		
		initialize(BufferType);
	}


	Buffer(Buffer&& other) noexcept : buffer(other.buffer) {
		LOG::CTOR::debug(this, &other, std::format("moving buffer with id {}", buffer));
		other.buffer = 0;
	}

	~Buffer() {
		cleanup();
	}

	void initialize(GLuint target) {
		glCreateBuffers(1, &buffer);
		glBindBuffer(target, buffer);
		LOG::CLAS::debug<&Buffer<BufferType>::initialize>(this, &buffer, DY::std_format("created buffer with id {}", buffer));
	}

	void cleanup() {
		glDeleteBuffers(1, &buffer);
		buffer = 0;
		LOG::CLAS::debug<&Buffer<BufferType>::cleanup>(this, &buffer, DY::std_format("deleted buffer with id {}", buffer));
	}

protected:
	GLuint buffer = 0;

public:

	inline static auto CR = DY::ClassRegister<
		&initialize,
		&cleanup>(
			"initialize",
			"cleanup");
	DY::ObjectRegister<Buffer<BufferType>, decltype(buffer)> OR;

	inline static auto CB = DY::ClassBinder(CR);
	inline static auto OB = DY::ObjectBinder<decltype(OR)>();

	using LOG = _LOG<decltype(CB), decltype(OB), DY::No_FB, DY::No_VB>;
};

using ArrayBuffer = Buffer<GL_ARRAY_BUFFER>;
using ElementArrayBuffer = Buffer<GL_ELEMENT_ARRAY_BUFFER>;
using ShaderStorageBuffer = Buffer<GL_SHADER_STORAGE_BUFFER>;
using QueryBuffer = Buffer<GL_QUERY_BUFFER>;
using TextureBuffer = Buffer<GL_TEXTURE_BUFFER>;
using InternalUniformBuffer = Buffer<GL_UNIFORM_BUFFER>;
