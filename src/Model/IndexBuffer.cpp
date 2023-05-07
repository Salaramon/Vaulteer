#include "vpch.h"
#include "Model/IndexBuffer.h"


IndexBuffer::IndexBuffer() {
}

IndexBuffer::IndexBuffer(GLuint vao) {
	bindVertexArray(vao);
}

IndexBuffer::IndexBuffer(const std::vector<GLuint>& indices, GLuint vao) {
	bindVertexArray(vao);
	insert(indices);
}

IndexBuffer::IndexBuffer(IndexBuffer&& other) noexcept : Buffer(std::move(other)) {
}


void IndexBuffer::insert(const std::vector<GLuint>& indices) {
	glNamedBufferData(buffer, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
}

void IndexBuffer::insertPartial(size_t position, const std::vector<GLuint>& indices) {
	glNamedBufferSubData(buffer, position * sizeof(GLuint), indices.size() * sizeof(GLuint), indices.data());
}

void IndexBuffer::reserve(size_t bufferSize) const {
	glNamedBufferStorage(buffer, bufferSize * sizeof(GLuint), nullptr, GL_DYNAMIC_STORAGE_BIT);
}

void IndexBuffer::bindVertexArray(GLuint vao) {
	glVertexArrayElementBuffer(vao, buffer);
}
