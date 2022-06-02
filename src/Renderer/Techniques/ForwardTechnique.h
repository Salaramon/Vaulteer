#pragma once

#include "Renderer/ShaderProgram.h"

class ForwardTechnique : public ShaderProgram<ForwardShader>
{
public:
	static void setModelView(const glm::dmat4& model, const glm::dmat4& view);
	static void setProjection(const glm::mat4& matrix);
	static void setNormal(const glm::mat4& matrix);
	static void setInverseViewMatrix(const glm::mat4& view);
};

