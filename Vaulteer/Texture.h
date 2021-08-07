#pragma once

#include <string>
#include <iostream>
#include <unordered_map>

#include<glad/glad.h>
#include<assimp/scene.h>

#include "stb_image.h"
#include "GLSLCPPBinder.h"

#include "DebugLogger.h"

class Texture : public DebugLogger<Texture>
{
public:
	Texture(std::string path, Binder::UniformInfo uniform);

	Binder::UniformInfo uniform;
	GLuint textureID;
	GLint width, height;
	GLint nrComponents;
	aiTextureType textureType;

	//Need to make the binder return the array size of uniforms.
	//Make a seperate file that contains the array for vertexAttribute
	//Make field in binder that will not be overwritten. JUST MAYBE

	inline static std::unordered_map<aiTextureType, Binder::UniformInfo> uniformTextureTypes;
private:
	void initialize(std::string path, Binder::UniformInfo uniform);
	
	void loadTexture(std::string path);
};

