#include "vpch.h"
#include "Model/ElementBuffer.h"

ElementBuffer::ElementBuffer() {
	LOG::CTOR::debug(this, &buffer, std::format("Empty ElementBuffer created with id {}", buffer));
}

ElementBuffer::ElementBuffer(size_t bufferSize) {
	reserve(bufferSize);

	LOG::CTOR::debug(this, &buffer, std::format("ElementBuffer created with id {} with size {}", buffer, bufferSize));
}


ElementBuffer::ElementBuffer(size_t bufferSize, GLuint vao) {
	reserve(bufferSize);
	bindVertexArray(vao);

	LOG::CTOR::debug(this, &buffer, std::format("ElementBuffer created with id {} with size {} and vao with id {}", buffer, bufferSize, vao));
}

ElementBuffer::ElementBuffer(const std::vector<Index>& indices) {
	insert(indices);

	LOG::CTOR::debug(this, &buffer, std::format("ElementBuffer created with id {} from indices", buffer));
}

ElementBuffer::ElementBuffer(const std::vector<Index>& indices, GLuint vao) {
	insert(indices);
	bindVertexArray(vao);

	LOG::CTOR::debug(this, &buffer, std::format("ElementBuffer created with id {} form indices and vao with id {}", buffer, vao));
}

ElementBuffer::ElementBuffer(ElementBuffer&& other) noexcept :
	Buffer(std::move(other)) {
	//debug("ElementBuffer moved. Buffer: " + std::to_string(buffer) + "\n");
	LOG::CTOR::debug(this, &other, std::format("ElementBuffer created with id {} was moved", buffer));
}


void ElementBuffer::insert(const std::vector<Index>& indices) {
	glNamedBufferData(buffer, indices.size() * sizeof(Index), indices.data(), GL_STATIC_DRAW);
	LOG::CLAS::debug<&ElementBuffer::insert>(this, &buffer, std::format("Indices inserted into buffer with id {}", buffer));
}

void ElementBuffer::insertPartial(size_t position, const std::vector<Index>& indices) {
	glNamedBufferSubData(buffer, position * sizeof(Index), indices.size() * sizeof(Index), indices.data());
	LOG::CLAS::debug<&ElementBuffer::insertPartial>(this, &buffer, std::format("Partial insert of indices into ElementBuffer with id {}", buffer));
}

void ElementBuffer::reserve(size_t bufferSize) const {
	glNamedBufferStorage(buffer, bufferSize * sizeof(Index), nullptr, GL_DYNAMIC_STORAGE_BIT);
	LOG::CLAS::debug<&ElementBuffer::reserve>(this, &buffer, std::format("element buffer of id {}, size set to {}", buffer, bufferSize * sizeof(Index)));
}

void ElementBuffer::bindVertexArray(GLuint vao) {
	glVertexArrayElementBuffer(vao, buffer);
	LOG::CLAS::debug<&ElementBuffer::bindVertexArray>(this, &buffer, std::format("Bound to VertexArray of id: {} to ElementBuffer of id {}", vao, buffer));
}
