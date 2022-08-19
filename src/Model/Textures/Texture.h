#pragma once

#include <unordered_map>

#include <glad/glad.h>
#include <assimp/scene.h>

// ReSharper disable once CppUnusedIncludeDirective
#include "stb_image.h"

#include "GLSLCPPBinder.h"
#include "Renderer/Shader.h"

#include "Debug/DebugLogger.h"

class Texture : public DebugLogger<Texture> {
public:
	Texture() = default;
	Texture(bool mipmapEnabled);
	Texture(GLsizei width, GLsizei height, bool mipmapEnabled = true);

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

	static std::pair<GLint, GLint> getFormatsFromComponents(int nrComponents);
};
