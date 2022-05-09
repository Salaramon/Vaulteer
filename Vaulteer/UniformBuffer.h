#pragma once

#include <any>
#include <iostream>

#include "GLSLCPPBinder.h"
#include "ModelData.h"

class UniformBuffer : Buffer<BufferType::UniformBuffer> {
public:
	struct DrawHint {
		inline static constexpr GLenum Dynamic = GL_DYNAMIC_DRAW;	// modify repeatedly, use often
		inline static constexpr GLenum Static  = GL_STATIC_DRAW;	// modify once, use often
		inline static constexpr GLenum Stream  = GL_STREAM_DRAW;	// modify once, use rarely
	};

	UniformBuffer(Binder::UniformBufferInfo bufferInfo, GLenum hint = DrawHint::Dynamic);
	UniformBuffer(UniformBuffer&& other) noexcept;

	template<class T>
	void insert(std::vector<T>& data) {
		size_t dataSize = data.size() * sizeof(data[0]);
		if (dataSize > size)
			std::cout << "WARNING: Attempted to push too much data to UniformBuffer " << buffer << " from vector: " << size << " <- " << dataSize << std::endl;

		glBindBufferBase(GL_UNIFORM_BUFFER, binding, buffer);
		glNamedBufferData(buffer, std::min(size, dataSize), data.data(), drawHint);
	}

	template<class T>
	void insert(T& data) {
		size_t dataSize = sizeof(data);
		if (dataSize > size)
			std::cout << "WARNING: Attempted to push too much data to UniformBuffer " << buffer << ": " << size << " <- " << dataSize << std::endl;

		glBindBufferBase(GL_UNIFORM_BUFFER, binding, buffer);
		glNamedBufferData(buffer, std::min(size, dataSize), data, drawHint);
	}

private:
	const size_t binding;
	const size_t size;

	const GLenum drawHint;
};

