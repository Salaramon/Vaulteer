#pragma once

#include "Renderer/ShaderProgram.h"
#include "Renderer/UniformBuffer.h"

class ForwardTechnique : public ShaderProgram<ForwardShader>
{
public:
	static void setModelView(const glm::dmat4& model, const glm::dmat4& view);
	static void setNormal(const glm::mat4& matrix);
	static void setInverseViewMatrix(const glm::mat4& view);

	static void setTextureUnit(const GLint& texture);

	// uniform buffer upload
	static void uploadProjection(const glm::mat4& projection);
	static void uploadModelUnitTables(const std::vector<ModelData*>& modelVector);
	static void uploadMaterialData(const std::vector<ModelData*>& modelVector);

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

