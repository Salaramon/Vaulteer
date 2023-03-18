#pragma once

#include "Model/Model.h"
#include "Scene/Scene.h"
#include "Renderer/Buffers/UniformBuffer.h"

class DeferredGeometryTechnique
{
public:
	//virtual void setModel(const glm::mat4& model);
	//void setView(const glm::mat4& view);
	static void setModel(const glm::dmat4& model);
	static void setModelView(const glm::dmat4& model, const glm::dmat4& view);
	static void setNormal(const glm::mat4& normal);
	
	static void setTextureUnit(const GLint& texture);
	//void setModelNumber(const GLint& modelNumber);

	// uniform buffer upload
	static void uploadProjection(const glm::mat4& projection);
	static void uploadModelUnitTables(const ViewCollection<ModelUnitTable, PropertiesModel>& modelUnitTableView);

	static UniformBuffer& getUBCamera() {
		static UniformBuffer camera = UniformBuffer(Binder::geometry_vertex::buffers::Camera);
		return camera;
	}

	static UniformBuffer& getUBModelUnitTables() {
		static UniformBuffer modelUnitTables = UniformBuffer(Binder::geometry_frag::buffers::ModelUnitTables);
		return modelUnitTables;
	}
};

