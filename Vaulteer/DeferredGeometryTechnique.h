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
	static void setModel(const glm::dmat4& model);
	static void setModelView(const glm::dmat4& model, const glm::dmat4& view);
	static void setNormal(const glm::mat4& model);
	
	static void setTextureUnit(const GLint& texture);
	//void setModelNumber(const GLint& modelNumber);

	// uniform buffer upload
	static void uploadProjection(const glm::mat4& projection);
	static void uploadModelUnitTables(const std::vector<ModelData*>& modelVector);

	static UniformBuffer& getUBCamera() {
		static UniformBuffer camera = UniformBuffer(Binder::geometry_vertex::buffers::Camera);
		return camera;
	}

	static UniformBuffer& getUBModelUnitTables() {
		static UniformBuffer modelUnitTables = UniformBuffer(Binder::geometry_frag::buffers::ModelUnitTables);
		return modelUnitTables;
	}
};

