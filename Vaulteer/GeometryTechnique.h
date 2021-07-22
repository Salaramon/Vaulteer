#pragma once
#include "Technique.h"
class GeometryTechnique :
    public Technique
{
public:
	GeometryTechnique(std::string pathGLSLVertexCode, std::string pathGLSLFragmentCode);

	void setModel(const glm::mat4& model);
	void setView(const glm::mat4& view);
	void setProjection(const glm::mat4& projection);
	void setNormal(const glm::mat4& model);

	void setTexture(const GLuint& texture);


};

