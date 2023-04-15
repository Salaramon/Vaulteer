#include "vpch.h"
#include "BlendingTechnique.h"

void BlendingTechnique::setModel(const glm::dmat4& model) {
	//shader->setUniform(Binder::blending_vertex::uniforms::model, 1, GL_FALSE, model);
	setNormal(model);
}

void BlendingTechnique::setView(const glm::dmat4& view) {
	//shader->setUniform(Binder::blending_vertex::uniforms::view, 1, GL_FALSE, view);
}

void BlendingTechnique::setNormal(const glm::mat4& normal) {
	//shader->setUniform(Binder::blending_vertex::uniforms::normal, 1, GL_FALSE, glm::inverse(normal));
}

void BlendingTechnique::setInverseViewMatrix(const glm::mat4& view) {
	//shader->setUniform(Binder::blending_frag::uniforms::inverseViewMat, 1, GL_FALSE, glm::inverse(view));
}

void BlendingTechnique::setTextureUnit(const GLint& texture) {
	//shader->setUniform(Binder::blending_frag::uniforms::textureLib, texture);
}


void BlendingTechnique::uploadProjection(const glm::mat4& projection) {
	auto& camera = getUBCamera();
	UniformBuffer::insert(camera, projection);
}

void BlendingTechnique::uploadModelUnitTables(const ViewCollection<ModelUnitTable, PropertiesModel>& modelUnitTableView) {
	/*
	std::vector<ModelUnitTable::ModelUnitData> unitData(MaterialLibrary::size() * 3);
	modelUnitTableView.each([&](ModelUnitTable& modelUnitTable, Material& material) {
		unitData[material.materialIndex * 3] = modelUnitTable.diffuseUnit;
		unitData[material.materialIndex * 3 + 1] = modelUnitTable.specularUnit;
		unitData[material.materialIndex * 3 + 2] = modelUnitTable.normalMapUnit;
		});

	auto& modelUnitTables = getUBModelUnitTables();
	UniformBuffer::insert(modelUnitTables, unitData);
	*/
}

void BlendingTechnique::uploadMaterialData() {
	std::vector<Material::MaterialData> materials;
	for (const auto& pair : MaterialLibrary::getAllMaterials()) {
		materials.push_back(pair.second->data);
	}
	auto& materialData = getUBMaterialData();
	UniformBuffer::insert(materialData, materials);
}
