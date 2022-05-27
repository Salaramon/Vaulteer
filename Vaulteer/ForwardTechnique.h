#pragma once

#include "ShaderProgram.h"

class ForwardTechnique : public ShaderProgram<ForwardShader>
{
public:
	static void setModel(glm::mat4 matrix);
	static void setView(glm::mat4 matrix);
	static void setProjection(glm::mat4 matrix);
};

