#pragma once

#include "ShaderProgram.h"
#include "Model.h"
#include "Scene.h"
#include "UniformBuffer.h"

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
	//void setModelNumber(const GLint& modelNumber);

	// uniform buffer upload
	void uploadModelUnitTables(const std::vector<ModelData*>& modelVector);

	UniformBuffer modelUnitTables = UniformBuffer(Binder::geometry_frag::buffers::ModelUnitTables);
};

