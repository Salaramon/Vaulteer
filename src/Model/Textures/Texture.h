#pragma once

#include <glad/glad.h>

class Texture {
public:
	Texture() = default;
	Texture(bool mipmapEnabled) : mipmapEnabled(mipmapEnabled) {}
	Texture(GLsizei width, GLsizei height, bool mipmapEnabled = true) : w(width), h(height), mipmapEnabled(mipmapEnabled) {}

	void setMinifyingFilter(GLenum filter) const;
	void setMagnifyingFilter(GLenum filter) const;
	void setAnisotropicFilter(GLfloat filter) const;

	GLuint textureID = 0;
	GLsizei w = -1, h = -1;
	bool mipmapEnabled = false;

protected:
	void createTexture(GLenum type);
	void cleanup() const;
};
