#pragma once

#include <glad/glad.h>
#include <type_traits>

#include "Debug/Debug.h"

struct BufferType {
	inline static constexpr GLenum ArrayBuffer = GL_ARRAY_BUFFER;
	inline static constexpr GLenum ElementBuffer = GL_ELEMENT_ARRAY_BUFFER;
	inline static constexpr GLenum ShaderStorageBuffer = GL_SHADER_STORAGE_BUFFER;
	inline static constexpr GLenum QueryBuffer = GL_QUERY_BUFFER;
	inline static constexpr GLenum TextureBuffer = GL_TEXTURE_BUFFER;
	inline static constexpr GLenum UniformBuffer = GL_UNIFORM_BUFFER;
};

template<GLenum bufferType>
class Buffer {
public:
	Buffer() :
		OR(this, DY::V(&buffer), DY::N("buffer"))
	{
		OB.add(OR);

		initialize(bufferType);
	}

	Buffer(Buffer&& other) noexcept : 
		OR(other.OR),

		buffer(other.buffer) {
		LOG::CTOR::debug(this, &other, std::format("moving buffer with id {}", buffer));
		other.buffer = 0;
	}

	~Buffer() {
		cleanup();
	}

	void initialize(GLuint target) {
		LOG::CLAS::debug<&Buffer<bufferType>::initialize>(this, &buffer, std::format("creating buffer with id {}", buffer));
		glCreateBuffers(1, &buffer);
		glBindBuffer(target, buffer);
	}

	void cleanup() {
		LOG::CLAS::debug<&Buffer<bufferType>::cleanup>(this, &buffer, std::format("deleting buffer with id {}", buffer));
		glDeleteBuffers(1, &buffer);
	}

protected:
	GLuint buffer = 0;

public:

	inline static auto CR = DY::ClassRegister<
		&initialize,
		&cleanup>(
			"initialize",
			"cleanup");
	DY::ObjectRegister<Buffer<bufferType>, decltype(buffer)> OR;

	inline static auto CB = DY::ClassBinder(CR);
	inline static auto OB = DY::ObjectBinder<decltype(OR)>();

	using LOG = _LOG<decltype(CB), decltype(OB), DY::No_FB, DY::No_VB>;
};
