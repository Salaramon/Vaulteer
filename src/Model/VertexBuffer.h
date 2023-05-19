#pragma once

#include <vector>

#include "Model/Buffer.h"
#include "Model/Storage/Vertex.h"
#include "Model/Storage/BufferLayout.h"

class VertexBuffer : public ArrayBuffer {
public:
	operator GLuint() const { return buffer; }


	VertexBuffer(GLuint vao, const BufferLayout& format) : format(format) {
		bindVertexArray(vao);
	}

	VertexBuffer(GLuint vao, const BufferLayout& format, size_t bufferSize) : format(format) {
		bindVertexArray(vao);
		reserve(bufferSize);
	}
	 
	VertexBuffer(VertexBuffer& other) = delete;

	VertexBuffer(VertexBuffer&& other) noexcept : Buffer(std::move(other)), format(other.format) {
		other.buffer = 0;
	}


	void bindVertexArray(GLuint vao, GLuint bindIndex = 0) {
		glVertexArrayVertexBuffer(vao, bindIndex, buffer, 0, getFormat().getStride());
	}

	template<class T>
	void insert(const std::vector<T>& vertices) const {
		static_assert(std::is_base_of_v<Vertex, T>, "Cannot insert non-vertex type into vertex buffer.");
		glNamedBufferData(buffer, vertices.size() * format.getStride(), vertices.data(), GL_STATIC_DRAW);
	}
	
	template<class T>
	void insertPartial(size_t position, const std::vector<T>& vertices) const {
		static_assert(std::is_base_of_v<Vertex, T>, "Cannot insert non-vertex type into vertex buffer.");
		glNamedBufferSubData(buffer, position * format.getStride(), vertices.size() * format.getStride(), vertices.data());
	}

	void insert(const void* data, size_t count) const {
		glNamedBufferData(buffer, count * format.getStride(), data, GL_STATIC_DRAW);
	}
	
	void insertPartial(size_t position, const void* data, size_t count) const {
		glNamedBufferSubData(buffer, position * format.getStride(), count * format.getStride(), data);
	}
	
	void reserve(size_t bufferSize) const {
		glNamedBufferStorage(buffer, bufferSize * format.getStride(), nullptr, GL_DYNAMIC_STORAGE_BIT);
	}

	GLuint getBuffer() const { return buffer; }

	const BufferLayout& getFormat() const { return format; }

private:
	const BufferLayout& format;
};
