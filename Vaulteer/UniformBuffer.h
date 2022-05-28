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
	static void insert(UniformBuffer& ubo, const std::vector<T>& data) {
		size_t dataSize = data.size() * sizeof(data[0]);
		/*if (dataSize > ubo.size)
			std::cout << "WARNING: Attempted to push too much data to UniformBuffer " << ubo.buffer << " from vector: " << ubo.size << " <- " << dataSize << std::endl; */
		assert(dataSize <= ubo.size);

		glBindBufferBase(GL_UNIFORM_BUFFER, ubo.binding, ubo.buffer);
		glNamedBufferData(ubo.buffer, std::min(ubo.size, dataSize), data.data(), ubo.drawHint);
	}

	template<class T>
	static void insert(UniformBuffer& ubo, const T& data) {
		size_t dataSize = sizeof(data);
		/*if (dataSize > ubo.size)
			std::cout << "WARNING: Attempted to push too much data to UniformBuffer " << ubo.buffer << ": " << ubo.size << " <- " << ubo.dataSize << std::endl;*/
		assert(dataSize <= ubo.size);

		glBindBufferBase(GL_UNIFORM_BUFFER, ubo.binding, ubo.buffer);
		glNamedBufferData(ubo.buffer, std::min(ubo.size, dataSize), &data, ubo.drawHint);
	}

private:
	const size_t binding;
	const size_t size;

	const GLenum drawHint;
};

