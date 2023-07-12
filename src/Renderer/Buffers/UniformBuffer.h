#pragma once

#include <any>

#include "Model/Buffer.h"


struct UniformBlock {
	std::string name;
	GLint binding;
	GLsizei size;

	GLuint programSource;
};

class UniformBuffer : InternalUniformBuffer {
public:
	const std::string name;
	const size_t binding;
	const GLsizei size;

	const GLenum drawHint;


	struct DrawHint {
		inline static constexpr GLenum Dynamic = GL_DYNAMIC_DRAW; // modify repeatedly, use often
		inline static constexpr GLenum Static  = GL_STATIC_DRAW; // modify once, use often
		inline static constexpr GLenum Stream  = GL_STREAM_DRAW; // modify once, use rarely
	};


	UniformBuffer(const UniformBlock& block, GLenum hint = DrawHint::Dynamic) : name(block.name), binding(block.binding), size(block.size), drawHint(hint) {
		glNamedBufferData(buffer, size, nullptr, drawHint);
	}

	UniformBuffer(UniformBuffer&& other) noexcept : Buffer(std::move(other)), binding(other.binding), size(other.size), drawHint(other.drawHint) {
		other.buffer = 0;
	}

	template<class T>
	void insert(const std::vector<T>& data) {
		GLsizei dataSize = data.size() * sizeof(data[0]);
		
		KYSE_ASSERT(dataSize <= size);

 		glBindBufferBase(GL_UNIFORM_BUFFER, binding, buffer);
		glNamedBufferSubData(buffer, 0, dataSize, data.data());
	}

	template<class T>
	void insert(const T& data) {
		GLsizei dataSize = sizeof(data);

		KYSE_ASSERT(dataSize <= size);

		glBindBufferBase(GL_UNIFORM_BUFFER, binding, buffer);
		glNamedBufferSubData(buffer, 0, dataSize, &data);
	}

	template<class T>
	void insertAt(const T& data, size_t offset) {
		GLsizei dataSize = sizeof(data);

		KYSE_ASSERT(offset + dataSize <= size);

		glBindBufferBase(GL_UNIFORM_BUFFER, binding, buffer);
		glNamedBufferSubData(buffer, offset, dataSize, &data);
	}

	UniformBuffer(UniformBuffer& other) = delete;
	UniformBuffer(const UniformBuffer& other) = delete;
	UniformBuffer(const UniformBuffer&& other) = delete;

};

