#pragma once

#include <any>
#include <iostream>

#include "GLSLCPPBinder.h"
#include "ModelData.h"

class UniformBuffer {
public:
	enum DrawHint {
		Dynamic = GL_DYNAMIC_DRAW,	// modify repeatedly, use often
		Static  = GL_STATIC_DRAW,	// modify once, use often
		Stream  = GL_STREAM_DRAW,	// modify once, use rarely
	};

	UniformBuffer(Binder::UniformBufferInfo bufferInfo, DrawHint hint = DrawHint::Dynamic);
	UniformBuffer(UniformBuffer& other) noexcept;
	UniformBuffer(UniformBuffer&& other) noexcept;
	~UniformBuffer();

	template<class T>
	void setData(std::vector<T>& data) {
		size_t dataSize = data.size() * sizeof(data[0]);
		if (dataSize > size)
			std::cout << "WARNING: Attempted to push too much data to UniformBuffer " << UBO << " from vector: " << size << " <- " << dataSize << std::endl;

		glBindBufferBase(GL_UNIFORM_BUFFER, binding, UBO);
		glNamedBufferData(UBO, std::min(size, dataSize), data.data(), drawHint);
	}

	template<class T>
	void setData(T& data) {
		size_t dataSize = sizeof(data);
		if (dataSize > size)
			std::cout << "WARNING: Attempted to push too much data to UniformBuffer " << UBO << ": " << size << " <- " << dataSize << std::endl;

		glBindBufferBase(GL_UNIFORM_BUFFER, binding, UBO);
		glNamedBufferData(UBO, std::min(size, dataSize), data, drawHint);
	}

private:
	GLuint UBO;
	const size_t binding;
	const size_t size;

	const DrawHint drawHint;
};

