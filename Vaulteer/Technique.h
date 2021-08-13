#pragma once
#include <glm/glm.hpp>

#include "Shader.h"

class Technique : public Shader
{
public:
	using Shader::Shader;

	virtual void init() {};
	void use();

protected:
};
