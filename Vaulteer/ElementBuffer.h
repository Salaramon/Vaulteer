#pragma once

#include <vector>

#include "Buffer.h"
#include "VertexArray.h"

#include "DebugLogger.h"

typedef GLuint Index;

class ElementBuffer : public Buffer<BufferType::ElementBuffer>, public DebugLogger<ElementBuffer> {
public:
	ElementBuffer();

	ElementBuffer(size_t bufferSize) {} // TODO not impl

	ElementBuffer(std::vector<Index>& indices);

	ElementBuffer(std::vector<Index>& indices, VertexArray& vertexArray);

	ElementBuffer(ElementBuffer&& other) noexcept;

	void insert(const std::vector<Index>& indices);

	void insertPartial(size_t position, const std::vector<Index>& indices);

	void bindVertexArray(VertexArray& vertexArray);
};





