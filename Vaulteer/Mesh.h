#pragma once

#include <vector>

#include <glad/glad.h>

#include "Vertex.h"
#include "Texture.h"
#include "Renderable.h"

class Mesh : public Renderable
{
public:

	Mesh();
	Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> textures);

	void draw(Shader& shader) override;

private:
	GLuint VAO, VBO, EBO;

	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<Texture> textures;

	void setup();
};

