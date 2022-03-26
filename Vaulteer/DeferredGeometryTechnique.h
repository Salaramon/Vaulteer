#pragma once
#include "ShaderProgram.h"
#include "ModelData.h"

class DeferredGeometryTechnique : public ShaderProgram<GeometryShader>
{
public:
	//virtual void setModel(const glm::mat4& model);
	//void setView(const glm::mat4& view);
	void setModel(const glm::dmat4& model);
	void setModelView(const glm::dmat4& model, const glm::dmat4& view);
	void setProjection(const glm::mat4& projection);
	void setNormal(const glm::mat4& model);

	void setTextureUnit(const GLint& texture);
	void setModelNumber(const GLint& modelNumber);
	void setModelUnitTable(const ModelData::ModelUnitTable& table);


};

