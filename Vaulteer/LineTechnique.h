#pragma once

#include "ShaderProgram.h"

class LineTechnique : public ShaderProgram<LineShader>
{
public:
	void setModel(glm::mat4 matrix);
	void setView(glm::mat4 matrix);
	void setProjection(glm::mat4 matrix);
	void setColor(glm::vec4 matrix);
};

