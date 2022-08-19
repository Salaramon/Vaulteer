#pragma once

#include <vector>

#include "Model/Buffer.h"
#include "Model/VertexArray.h"

#include "Debug/DebugLogger.h"

typedef GLuint Index;

class ElementBuffer : public Buffer<BufferType::ElementBuffer>, public DebugLogger<ElementBuffer> {
public:
	ElementBuffer();

	ElementBuffer(size_t bufferSize);

	ElementBuffer(size_t bufferSize, GLuint vao);

	ElementBuffer(const std::vector<Index>& indices);

	ElementBuffer(const std::vector<Index>& indices, GLuint vao);

	ElementBuffer(ElementBuffer&& other) noexcept;

	void insert(const std::vector<Index>& indices);

	void insertPartial(size_t position, const std::vector<Index>& indices);

	void reserve(size_t bufferSize) const;

	void bindVertexArray(GLuint vao);
};
