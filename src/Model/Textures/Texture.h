#pragma once

#include <unordered_map>

#include <glad/glad.h>
#include <assimp/scene.h>

#include "stb_image.h"

#include "Renderer/Shader.h"

class Texture {
public:
	Texture() = default;
	Texture(bool mipmapEnabled) : mipmapEnabled(mipmapEnabled) {}
	Texture(GLsizei width, GLsizei height, bool mipmapEnabled = true) : width(width), height(height), mipmapEnabled(mipmapEnabled) {}

	void setMinifyingFilter(GLenum filter) const;
	void setMagnifyingFilter(GLenum filter) const;
	void setAnisotropicFilter(GLfloat filter) const;

	GLuint textureID = 0;
	GLsizei width = -1, height = -1;
	GLint nrComponents = -1;
	bool mipmapEnabled = false;

protected:
	void createTexture(GLenum type);
	void cleanup() const;
};
