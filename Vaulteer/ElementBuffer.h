#pragma once

#include <vector>

#include "Buffer.h"
#include "VertexArray.h"

#include "DebugLogger.h"

typedef GLuint Index;

class ElementBuffer : public Buffer, public DebugLogger<ElementBuffer>
{
public:
	ElementBuffer();

	ElementBuffer(std::vector<Index>& indices);

	ElementBuffer(std::vector<Index>& indices, VertexArray& vertexArray);

	ElementBuffer(ElementBuffer&& other) noexcept;

	void insert(const std::vector<Index>& indices);

	void bindVertexArray(VertexArray& vertexArray);
};





