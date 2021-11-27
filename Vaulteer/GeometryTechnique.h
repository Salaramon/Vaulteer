#pragma once
#include "Technique.h"
class GeometryTechnique :
    public Technique
{
public:
	using Technique::Technique;

	//virtual void setModel(const glm::mat4& model);
	//void setView(const glm::mat4& view);
	void setModelView(const glm::dmat4& model, const glm::dmat4& view);
	void setProjection(const glm::mat4& projection);
	void setNormal(const glm::mat4& model);

	void setTexture(const GLint& texture);


};

