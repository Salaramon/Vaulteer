#pragma once

#include "ShaderProgram.h"

class ForwardTechnique : public ShaderProgram<ForwardShader>
{
public:
	void setModel(glm::mat4 matrix);
	void setView(glm::mat4 matrix);
	void setProjection(glm::mat4 matrix);
};

