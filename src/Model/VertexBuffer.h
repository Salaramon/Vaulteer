#pragma once

#include <vector>

#include "Model/Buffer.h"
#include "Model/Storage/Vertex.h"
#include "Model/Storage/VertexBufferLayout.h"

class VertexBuffer : public ArrayBuffer {
public:
	operator GLuint() const { return buffer; }


	VertexBuffer(GLuint vao, size_t stride, GLuint bindIndex = 0) : stride(stride), size(0) {
		bindVertexArray(vao, bindIndex);
	}

	VertexBuffer(VertexBuffer& other) = delete;

	VertexBuffer(VertexBuffer&& other) noexcept : Buffer(std::move(other)), stride(other.stride), size(other.size) {
		other.buffer = 0;
	}


	void bindVertexArray(GLuint vao, GLuint bindIndex = 0) {
		glVertexArrayVertexBuffer(vao, bindIndex, buffer, 0, stride);
	}

	template<class T>
	void insert(const std::vector<T>& vertices, GLenum drawHint = GL_STATIC_DRAW) {
		static_assert(std::is_base_of_v<Vertex, T>, "Cannot insert non-vertex type into vertex buffer.");
		glNamedBufferData(buffer, vertices.size() * stride, vertices.data(), drawHint);
		size = vertices.size() * stride;
	}

	void insert(const void* data, size_t count, GLenum drawHint = GL_STATIC_DRAW) {
		glNamedBufferData(buffer, count * stride, data, drawHint);
		size = count * stride;
	}
	
	template<class T>
	void insertPartial(size_t position, const std::vector<T>& vertices) const {
		static_assert(std::is_base_of_v<Vertex, T>, "Cannot insert non-vertex type into vertex buffer.");
		KYSE_ASSERT(size >= position * stride + vertices.size() * stride, "Attempted to insert data beyond reserved space");
		glNamedBufferSubData(buffer, position * stride, vertices.size() * stride, vertices.data());
	}
	
	void insertPartial(size_t position, const void* data, size_t count) const {
		KYSE_ASSERT(size >= position * stride + count * stride, "Attempted to insert data beyond reserved space");
		glNamedBufferSubData(buffer, position * stride, count * stride, data);
	}
	
	void reserve(size_t bufferSize) {
		glNamedBufferStorage(buffer, bufferSize * stride, nullptr, GL_DYNAMIC_STORAGE_BIT);
		size = bufferSize * stride;
	}

	GLuint getBuffer() const { return buffer; }

	const size_t stride;
	size_t size;
};
