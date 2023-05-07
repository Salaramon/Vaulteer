#pragma once

#include <vector>

#include "Model/Buffer.h"

class IndexBuffer : public ElementArrayBuffer {
public:
	IndexBuffer();

	IndexBuffer(GLuint vao);

	IndexBuffer(const std::vector<GLuint>& indices);

	IndexBuffer(const std::vector<GLuint>& indices, GLuint vao);

	IndexBuffer(IndexBuffer& other) noexcept;

	IndexBuffer(IndexBuffer&& other) noexcept;

	void insert(const std::vector<GLuint>& indices);

	void insertPartial(size_t position, const std::vector<GLuint>& indices);

	void reserve(size_t bufferSize) const;

	void bindVertexArray(GLuint vao);
};
