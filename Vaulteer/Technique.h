#pragma once
#include <glm/glm.hpp>

#include "Shader.h"

class Technique
{
public:
	Technique(std::string pathGLSLVertexCode, std::string pathGLSLFragmentCode);

	virtual void init() {};
	void use();

protected:
	Shader shader;
};
