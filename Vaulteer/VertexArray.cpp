#include "VertexArray.h"

VertexArray::VertexArray(const VertexBuffer& vertexBuffer, const Vertices& vertices,const Indices& indices) : vertices(vertices), indices(indices)
{
	initialize(vertexBuffer);
}

VertexArray::~VertexArray()
{
	cleanup();
}

void VertexArray::initialize(const VertexBuffer& vertexBuffer)
{

	glGenVertexArrays(1, &VAO);
	bind();

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer.getVBO());
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), indices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexBuffer.getEBO());
	glBufferData(GL_ARRAY_BUFFER, indices.size() * sizeof(Vertex), indices.data(), GL_STATIC_DRAW);

	unbind();
}

GLuint VertexArray::getVAO() const
{
	return VAO;
}

void VertexArray::bind()
{
	if (boundArray == nullptr) {
		glBindVertexArray(VAO);
		boundArray = this;
	}
	else {
		std::cout << "VertexArray is already bound!" << std::endl;
	}
}

void VertexArray::unbind()
{
	glBindVertexArray(0);
	boundArray = nullptr;
}

void VertexArray::cleanup() {
	glDeleteVertexArrays(1, &VAO);
}