#pragma once

#include <unordered_map>

#include <glad/glad.h>
#include <assimp/scene.h>

#include "stb_image.h"

#include "GLSLCPPBinder.h"
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

	inline static std::unordered_map<aiTextureType, Binder::UniformInfo> uniformTextureTypes;
protected:
	void createTexture(GLenum type);
	void cleanup() const;
    
	static std::pair<GLint, GLint> getFormatsFromComponents(int nrComponents) {
	    switch (nrComponents) {
	        case STBI_rgb_alpha:	return std::make_pair(GL_RGBA8, GL_RGBA);
	        case STBI_rgb:			return std::make_pair(GL_RGB8, GL_RGB);
	        case STBI_grey_alpha:	return std::make_pair(GL_RG8, GL_RG);
	        default: assert(false); // "invalid format"
	        case STBI_grey:			return std::make_pair(GL_R8, GL_RED);
	    }
	}
};
