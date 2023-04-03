#include "vpch.h"
#include "Model/IndexBuffer.h"


IndexBuffer::IndexBuffer() {
	LOG::CTOR::debug(this, &buffer, std::format("IndexBuffer created with id {}", buffer));
}

IndexBuffer::IndexBuffer(GLuint vao) {
	bindVertexArray(vao);
	LOG::CTOR::debug(this, &buffer, std::format("IndexBuffer created with id {} and vao with id {}", buffer, vao));
}

IndexBuffer::IndexBuffer(const std::vector<GLuint>& indices, GLuint vao) {
	bindVertexArray(vao);
	insert(indices);

	LOG::CTOR::debug(this, &buffer, std::format("IndexBuffer created with id {} form indices and vao with id {}", buffer, vao));
}

IndexBuffer::IndexBuffer(IndexBuffer&& other) noexcept : Buffer(std::move(other)) {
	LOG::CTOR::debug(this, &other, std::format("IndexBuffer created with id {} was moved", buffer));
}


void IndexBuffer::insert(const std::vector<GLuint>& indices) {
	glNamedBufferData(buffer, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
	LOG::CLAS::debug<&IndexBuffer::insert>(this, &buffer, std::format("Indices inserted into buffer with id {}", buffer));
}

void IndexBuffer::insertPartial(size_t position, const std::vector<GLuint>& indices) {
	glNamedBufferSubData(buffer, position * sizeof(GLuint), indices.size() * sizeof(GLuint), indices.data());
	LOG::CLAS::debug<&IndexBuffer::insertPartial>(this, &buffer, std::format("Partial insert of indices into IndexBuffer with id {}", buffer));
}

void IndexBuffer::reserve(size_t bufferSize) const {
	glNamedBufferStorage(buffer, bufferSize * sizeof(GLuint), nullptr, GL_DYNAMIC_STORAGE_BIT);
	LOG::CLAS::debug<&IndexBuffer::reserve>(this, &buffer, std::format("element buffer of id {}, size set to {}", buffer, bufferSize * sizeof(GLuint)));
}

void IndexBuffer::bindVertexArray(GLuint vao) {
	glVertexArrayElementBuffer(vao, buffer);
	LOG::CLAS::debug<&IndexBuffer::bindVertexArray>(this, &buffer, std::format("Bound to VertexArray of id: {} to IndexBuffer of id {}", vao, buffer));
}
