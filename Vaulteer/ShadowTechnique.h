#pragma once

#include "Technique.h"

class ShadowTechnique : public Technique
{
public:
	ShadowTechnique(std::string pathGLSLVertexShader, std::string pathGLSLFragmentShader);

	void setModel(const glm::mat4& model);
	void setView(const glm::mat4& view);
	void setProjection(const glm::mat4& projection);

	void setShadowMap(GLuint textureUnit);

private:
};
