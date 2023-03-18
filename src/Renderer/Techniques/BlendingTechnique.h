#pragma once

#include "Renderer/Buffers/UniformBuffer.h"
#include "Model/Resources/MaterialLibrary.h"
#include "Model/Model.h"
#include "Scene/Scene.h"

class BlendingTechnique {
public:
	static void setModel(const glm::dmat4& model);
	static void setView(const glm::dmat4& view);
	static void setNormal(const glm::mat4& matrix);
	static void setInverseViewMatrix(const glm::mat4& view);

	static void setTextureUnit(const GLint& texture);

	// uniform buffer upload
	static void uploadProjection(const glm::mat4& projection);
	static void uploadModelUnitTables(const ViewCollection<ModelUnitTable, PropertiesModel>& modelUnitTableView);
	static void uploadMaterialData();

	static UniformBuffer& getUBCamera() {
		static UniformBuffer camera = UniformBuffer(Binder::forward_vertex::buffers::Camera);
		return camera;
	}

	static UniformBuffer& getUBModelUnitTables() {
		static UniformBuffer modelUnitTables = UniformBuffer(Binder::forward_frag::buffers::ModelUnitTables);
		return modelUnitTables;
	}

	static UniformBuffer& getUBMaterialData() {
		static UniformBuffer materialData = UniformBuffer(Binder::forward_frag::buffers::MaterialData);
		return materialData;
	}
};
