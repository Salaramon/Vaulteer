#pragma once

#include "ShaderProgram.h"

class ForwardTechnique : public ShaderProgram<ForwardShader>
{
public:
	void setModelView(const glm::dmat4& model, const glm::dmat4& view);
	void setProjection(const glm::mat4& matrix);
	void setNormal(const glm::mat4& matrix);
	void setInverseViewMatrix(const glm::mat4& view);
};

