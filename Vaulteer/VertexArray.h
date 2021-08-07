#pragma once

#include <vector>
#include <iostream>

#include <glad/glad.h>

#include "Vertex.h"
#include "GLSLCPPBinder.h"

typedef std::vector<Vertex> Vertices;
typedef std::vector<GLuint> Indices;

class VertexArray : public DebugLogger<VertexArray>
{
public:
	VertexArray();
	VertexArray(VertexArray&& other) : 
		VAO(other.VAO)
	{
		other.VAO = 0;
		debug("VertexArray moved. VAO: " + std::to_string(VAO) + "\n", "MOVE_CONSTRUCTOR");
	}
	~VertexArray();
	operator GLuint() const { return VAO; }

	void bind();
	void unbind();

private:
	void cleanup();
	void initialize();

	GLuint VAO;

	inline static VertexArray* currentlyBound = nullptr;
};

