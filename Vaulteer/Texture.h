#pragma once

#include <string>
#include <iostream>
#include <unordered_map>

#include<glad/glad.h>
#include<assimp/scene.h>

#include "stb_image.h"

#include "GLSLCPPBinder.h"

#include "Shader.h"

#include "DebugLogger.h"
#include "DebugAliases.h"

class Texture : public DebugLogger<Texture>
{
public:
	Texture(std::string path, Binder::UniformInfo uniform);
	Texture(Binder::UniformInfo uniform, GLsizei width, GLsizei height, std::vector<glm::vec4> colors);

	template<class... Args>
	requires (std::conjunction_v<std::is_same<glm::vec4, Args>...>)
	Texture(Binder::UniformInfo uniform, GLsizei width, GLsizei height, glm::vec4 first, Args ...args);

	Texture(Texture&& other);
	~Texture();

	void activate(const Shader& shader, GLint index);

	void setWrap(GLenum x, GLenum y);

	void setMinifyingFilter(GLenum filter);
	void setMagnifyingFilter(GLenum filter);

	void setAnisotropicFilter(GLfloat filter);

	Binder::UniformInfo uniform;
	GLuint textureID;
	GLsizei width, height;
	GLint nrComponents;
	aiTextureType textureType;

	Texture* currentlyBound = nullptr;

	inline static std::unordered_map<aiTextureType, Binder::UniformInfo> uniformTextureTypes;
private:
	
	void initialize();
	void cleanup();

	void loadTexture(std::string path);
	void loadTexture(std::vector<glm::vec4> colors);
};

template<class ...Args>
requires (std::conjunction_v<std::is_same<glm::vec4, Args>...>)
inline Texture::Texture(Binder::UniformInfo uniform, GLsizei width, GLsizei height, glm::vec4 first, Args ...args) : uniform(uniform), width(width), height(height)
{
	loadTexture({ first, args... });
}