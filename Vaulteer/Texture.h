#pragma once

#include <string>
#include <iostream>
#include <unordered_map>

#include<glad/glad.h>
#include<assimp/scene.h>

#include "stb_image.h"
#include "GLSLCPPBinder.h"

class Texture
{
public:
	Texture(std::string path, Uniform uniform);

	Uniform uniform;
	GLuint textureID;
	GLint width, height;
	GLint nrComponents;
	aiTextureType textureType;

	//Need to make the binder return the array size of uniforms.
	//Make a seperate file that contains the array for vertexAttribute
	//Make field in binder that will not be overwritten. JUST MAYBE

	inline static std::unordered_map<aiTextureType, Uniform> uniformTextureTypes;
private:
	void initialize(std::string path, Uniform uniform);
	
	void loadTexture(std::string path);
};

