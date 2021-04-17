#pragma once

#include <vector>

#include<glad/glad.h>

class VertexBuffer
{
public:
	VertexBuffer();
	~VertexBuffer();

	GLuint getVBO() const;
	GLuint getEBO() const;

private:
	void initialize();
	void cleanup();

	GLuint VBO;
	GLuint EBO;

};

