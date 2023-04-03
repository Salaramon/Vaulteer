#pragma once

#include <vector>

#include "Model/Buffer.h"
#include "Model/Storage/Vertex.h"
#include "Model/Storage/BufferLayout.h"

template<class T>
concept vertex_concept = std::is_base_of_v<Vertex, T>;

class VertexBuffer : public ArrayBuffer {
public:
	operator GLuint() const { return buffer; }

	VertexBuffer(BufferLayout format) : format(format) {}

	VertexBuffer(BufferLayout format, size_t bufferSize) : format(format) {
		reserve(bufferSize);
	}

	VertexBuffer(VertexBuffer& other) noexcept = delete;

	VertexBuffer(VertexBuffer&& other) noexcept : Buffer(std::move(other)), format(std::move(other.format)) {
		other.buffer = 0;
	}

	void bindVertexArray(GLuint vao) {
		glVertexArrayVertexBuffer(vao, 0, buffer, 0, getFormat().getStride());
	}

	template<vertex_concept T>
	void insert(const std::vector<T>& vertices) const {
		glNamedBufferData(buffer, vertices.size() * format.getStride(), vertices.data(), GL_STATIC_DRAW);
	}
	
	template<vertex_concept T>
	void insertPartial(size_t position, const std::vector<T>& vertices) const {
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
	const BufferLayout format;
};
