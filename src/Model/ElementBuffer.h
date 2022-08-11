#pragma once

#include <vector>

#include "Model/Buffer.h"
#include "Model/VertexArray.h"

typedef GLuint Index;

class ElementBuffer : public Buffer<BufferType::ElementBuffer> {
public:
	ElementBuffer();

	ElementBuffer(size_t bufferSize);

	ElementBuffer(size_t bufferSize, GLuint vao);

	ElementBuffer(std::vector<Index>& indices);

	ElementBuffer(std::vector<Index>& indices, GLuint vao);

	ElementBuffer(ElementBuffer&& other) noexcept;

	void insert(const std::vector<Index>& indices);

	void insertPartial(size_t position, const std::vector<Index>& indices);

	void reserve(size_t bufferSize);

	void bindVertexArray(GLuint vao);
};





