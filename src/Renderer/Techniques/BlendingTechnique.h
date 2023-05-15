#pragma once

#include "Renderer/Buffers/UniformBuffer.h"
#include "Model/Resources/MaterialLibrary.h"
#include "Model/Model.h"
#include "Renderer/ShaderProgram.h"
#include "Scene/Scene.h"

class BlendingTechnique {
public:
	static const Shader& shader() { return ShaderProgram::blendingShader(); }

	static void setModel(const glm::dmat4& model) {
		shader().setUniform(Binder::blending_vertex::uniforms::model, 1, GL_FALSE, model);
		setNormal(glm::inverse(model));
	}

	static void setView(const glm::dmat4& view) {
		shader().setUniform(Binder::blending_vertex::uniforms::view, 1, GL_FALSE, view);
	}

	static void setNormal(const glm::mat4& normal) {
		shader().setUniform(Binder::blending_vertex::uniforms::normal, 1, GL_FALSE, normal);
	}

	static void setInverseViewMatrix(const glm::mat4& view) {
		shader().setUniform(Binder::blending_frag::uniforms::inverseViewMat, 1, GL_FALSE, view);
	}

	static void setTextureUnit(const GLint& texture) {
		shader().setUniform(Binder::blending_frag::uniforms::textureLib, texture);
	}

	// uniform buffer upload
	// TODO should be handled elsewhere that can guarantee their state or else they're copied between a bunch of techniques

	static void uploadProjection(const glm::mat4& projection) {
		auto& camera = getUBCamera();
		UniformBuffer::insert(camera, projection);
	}

	static void uploadModelUnitTables(ViewCollection<TextureView, PropertiesModel>& modelUnitTableView) {
		std::vector<TextureView::TextureViewData> unitData(MaterialLibrary::size() * 3);
		/*
		for (auto& data : dataVector) {
			unitData[data->getMaterialIndex() * 3] = data->getModelUnitTable().diffuseUnit;
			unitData[data->getMaterialIndex() * 3 + 1] = data->getModelUnitTable().specularUnit;
			unitData[data->getMaterialIndex() * 3 + 2] = data->getModelUnitTable().normalMapUnit;
		}

		modelUnitTableView.each([&](const TextureViewTable& textureViewTable, const PropertiesModel& props) {
		    unitData[material.materialIndex * 3] = textureViewTable.diffuseView;                            
		    unitData[material.materialIndex * 3 + 1] = textureViewTable.specularUnit;                       
		    unitData[material.materialIndex * 3 + 2] = textureViewTable.normalMapUnit;                      
		});
		 */
		auto& modelUnitTables = getUBModelUnitTables();                                                   
		UniformBuffer::insert(modelUnitTables, unitData);

		//assert(false); // do the to do thing you monkey
	}

	static void uploadMaterialData() {
		auto& materialData = getUBMaterialData();
		UniformBuffer::insert(materialData, MaterialLibrary::getMaterialData());
	}


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
