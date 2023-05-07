#pragma once

#include <any>

#include "GLSLCPPBinder.h"

#include "Model/Buffer.h"

class UniformBuffer : InternalUniformBuffer {
public:
	struct DrawHint {
		inline static constexpr GLenum Dynamic = GL_DYNAMIC_DRAW; // modify repeatedly, use often
		inline static constexpr GLenum Static  = GL_STATIC_DRAW; // modify once, use often
		inline static constexpr GLenum Stream  = GL_STREAM_DRAW; // modify once, use rarely
	};

	UniformBuffer(const Binder::UniformBufferInfo& bufferInfo, GLenum hint = DrawHint::Dynamic) : binding(bufferInfo.binding), size(bufferInfo.size), drawHint(hint) {
		std::cout << std::format("Creating UBO for {} (size {})", (std::string)bufferInfo.name, bufferInfo.size) << std::endl;
	}

	UniformBuffer(UniformBuffer&& other) noexcept : Buffer(std::move(other)), binding(other.binding), size(other.size), drawHint(other.drawHint) {
		other.buffer = 0;
	}

	template<class T>
	static void insert(UniformBuffer& ubo, const std::vector<T>& data) {
		size_t dataSize = data.size() * sizeof(data[0]);
		
		assert(dataSize <= ubo.size);

		glBindBufferBase(GL_UNIFORM_BUFFER, ubo.binding, ubo.buffer);
		glNamedBufferData(ubo.buffer, std::min(ubo.size, dataSize), data.data(), ubo.drawHint);
	
	}

	template<class T>
	static void insert(UniformBuffer& ubo, const T& data) {
		size_t dataSize = sizeof(data);

		assert(dataSize <= ubo.size);

		glBindBufferBase(GL_UNIFORM_BUFFER, ubo.binding, ubo.buffer);
		glNamedBufferData(ubo.buffer, std::min(ubo.size, dataSize), &data, ubo.drawHint);
		
		//LOG::SPGL::debug<static_cast<void(*)(UniformBuffer&, const T&)>(&UniformBuffer::insert<T>), UniformBuffer>(
		//	std::format("Inserting {} bytes into UBO {}", dataSize, ubo.buffer)
		//);
	}

	UniformBuffer(UniformBuffer& other) = delete;
	UniformBuffer(const UniformBuffer& other) = delete;
	UniformBuffer(const UniformBuffer&& other) = delete;

private:
	const size_t binding;
	const size_t size;

	const GLenum drawHint;

};

