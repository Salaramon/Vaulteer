#include "vpch.h"
#include "ElementBuffer.h"

ElementBuffer::ElementBuffer() {
	debug("ElementBuffer created. Buffer: " + std::to_string(buffer) + "\n");
}

ElementBuffer::ElementBuffer(size_t bufferSize) {
	debug("ElementBuffer created. Buffer: " + std::to_string(buffer) + "\n");

	reserve(bufferSize);
}


ElementBuffer::ElementBuffer(size_t bufferSize, GLuint vao) {
	debug("ElementBuffer created. Buffer: " + std::to_string(buffer) + "\n");

	reserve(bufferSize);
	bindVertexArray(vao);
}

ElementBuffer::ElementBuffer(std::vector<Index>& indices) {
	debug("ElementBuffer created. Buffer: " + std::to_string(buffer) + "\n");
	insert(indices);
}

ElementBuffer::ElementBuffer(std::vector<Index>& indices, GLuint vao) {
	debug("ElementBuffer created. Buffer: " + std::to_string(buffer) + "\n");
	insert(indices);
	bindVertexArray(vao);
}

ElementBuffer::ElementBuffer(ElementBuffer&& other) noexcept :
	Buffer(std::move(other)) {
	debug("ElementBuffer moved. Buffer: " + std::to_string(buffer) + "\n");
}


void ElementBuffer::insert(const std::vector<Index>& indices) {
	glNamedBufferData(buffer, indices.size() * sizeof(Index), indices.data(), GL_STATIC_DRAW);
	debug("Indices inserted into buffer: " + std::to_string(buffer) + "\n");
}

void ElementBuffer::insertPartial(size_t position, const std::vector<Index>& indices) {
	glNamedBufferSubData(buffer, position * sizeof(Index), indices.size() * sizeof(Index), indices.data());
	debug("Indices inserted into buffer: " + std::to_string(buffer) + "\n");
}

void ElementBuffer::reserve(size_t bufferSize) {
	glNamedBufferStorage(buffer, bufferSize * sizeof(Index), nullptr, GL_DYNAMIC_STORAGE_BIT);
}

void ElementBuffer::bindVertexArray(GLuint vao) {
	glVertexArrayElementBuffer(vao, buffer);
	debug("Bound to VertexArray: " + std::to_string(vao) + " -- buf " + std::to_string(buffer), "glVertexArrayElementBuffer");
}
