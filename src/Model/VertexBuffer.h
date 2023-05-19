#pragma once

#include <vector>

#include "Model/Buffer.h"
#include "Model/Storage/Vertex.h"
#include "Model/Storage/BufferLayout.h"

class VertexBuffer : public ArrayBuffer {
public:
	operator GLuint() const { return buffer; }


	VertexBuffer(GLuint vao, size_t stride, GLuint bindIndex = 0) : stride(stride) {
		bindVertexArray(vao, bindIndex);
	}

	VertexBuffer(VertexBuffer& other) = delete;

	VertexBuffer(VertexBuffer&& other) noexcept : Buffer(std::move(other)), stride(other.stride) {
		other.buffer = 0;
	}


	void bindVertexArray(GLuint vao, GLuint bindIndex = 0) {
		glVertexArrayVertexBuffer(vao, bindIndex, buffer, 0, stride);
	}

	template<class T>
	void insert(const std::vector<T>& vertices, GLenum drawHint = GL_STATIC_DRAW) const {
		static_assert(std::is_base_of_v<Vertex, T>, "Cannot insert non-vertex type into vertex buffer.");
		glNamedBufferData(buffer, vertices.size() * stride, vertices.data(), drawHint);
	}
	
	template<class T>
	void insertPartial(size_t position, const std::vector<T>& vertices) const {
		static_assert(std::is_base_of_v<Vertex, T>, "Cannot insert non-vertex type into vertex buffer.");
		glNamedBufferSubData(buffer, position * stride, vertices.size() * stride, vertices.data());
	}

	void insert(const void* data, size_t count, GLenum drawHint = GL_STATIC_DRAW) const {
		glNamedBufferData(buffer, count * stride, data, drawHint);
	}
	
	void insertPartial(size_t position, const void* data, size_t count) const {
		glNamedBufferSubData(buffer, position * stride, count * stride, data);
	}
	
	void reserve(size_t bufferSize) const {
		glNamedBufferStorage(buffer, bufferSize * stride, nullptr, GL_DYNAMIC_STORAGE_BIT);
	}

	GLuint getBuffer() const { return buffer; }

private:
	const size_t stride;
};
