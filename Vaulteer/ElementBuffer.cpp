#include "ElementBuffer.h"

ElementBuffer::ElementBuffer()
{
	debug("ElementBuffer created. Buffer: " + std::to_string(buffer) + "\n");
	initialize();
}

ElementBuffer::ElementBuffer(std::vector<Index>& indices)
{
	debug("ElementBuffer created. Buffer: " + std::to_string(buffer) + "\n");
	initialize();
	insert(indices);
}

ElementBuffer::ElementBuffer(std::vector<Index>& indices, VertexArray& vertexArray)
{
	debug("ElementBuffer created. Buffer: " + std::to_string(buffer) + "\n");
	initialize();
	insert(indices);
	bindVertexArray(vertexArray);
}

void ElementBuffer::insert(const std::vector<Index>& indices)
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(Index), indices.data(), GL_STATIC_DRAW);
	debug("Indices inserted into buffer: " + std::to_string(buffer) + "\n");
}

void ElementBuffer::bindVertexArray(VertexArray& vertexArray)
{
	glVertexArrayElementBuffer(vertexArray, buffer);
	debug("Bound to VertexArray: " + std::to_string(vertexArray.getObjectKey()) + "\n", "glVertexArrayElementBuffer");
}

ElementBuffer::ElementBuffer(ElementBuffer&& other) noexcept :
	Buffer(std::move(other))
{
	debug("ElementBuffer moved. Buffer: " + std::to_string(buffer) + "\n");
}