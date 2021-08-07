#include "VertexArray.h"

VertexArray::VertexArray() {
	initialize();
	debug("VertexArray created VAO: " + std::to_string(VAO) + "\n", "DEFAULT_CONSTRUCTOR");
}

VertexArray::~VertexArray()
{
	cleanup();
	debug("VertexArray destroyed. VAO: " + std::to_string(VAO) + "\n", "DECONSTRUCTOR");
}

void VertexArray::bind()
{
	if(currentlyBound == nullptr){
		currentlyBound = this;
		glBindVertexArray(VAO);
	}
	else {
		debug("VertexArray is already bound! VAO: " + std::to_string(VAO) + "\n");
	}
}

void VertexArray::unbind()
{
	currentlyBound = nullptr;
	glBindVertexArray(0);
}

void VertexArray::initialize() {
	glGenVertexArrays(1, &VAO);
}

void VertexArray::cleanup() {
	glDeleteVertexArrays(1, &VAO);
}