#pragma once
#include <glm/glm.hpp>

#include "Renderer/Shader.h"

class Technique : public Shader
{
public:
	using Shader::Shader;

	virtual void init() {};
	void use();

	virtual void setModel(const glm::mat4& modelMat) {};

protected:
};
