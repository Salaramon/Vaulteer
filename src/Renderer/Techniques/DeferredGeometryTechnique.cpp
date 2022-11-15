#include "vpch.h"
#include "Renderer/Techniques/DeferredGeometryTechnique.h"

#include "Model/Resources/MaterialLibrary.h"

void DeferredGeometryTechnique::setModel(const glm::dmat4& model) {
    shader->setUniform(Binder::geometry_vertex::uniforms::model, 1, GL_FALSE, model);

}

void DeferredGeometryTechnique::setModelView(const glm::dmat4& model, const glm::dmat4& view) {
    glm::fmat4 modelView = view * model;
    shader->setUniform(Binder::geometry_vertex::uniforms::modelView, 1, GL_FALSE, modelView);
    setModel(model);
    setNormal(model);
}

void DeferredGeometryTechnique::setNormal(const glm::mat4& normal) {
    shader->setUniform(Binder::geometry_vertex::uniforms::normal, 1, GL_FALSE, glm::transpose(glm::inverse(normal)));
}

void DeferredGeometryTechnique::setTextureUnit(const GLint& texture) {
    shader->setUniform(Binder::geometry_frag::uniforms::textureLib, texture);
}

/*void DeferredGeometryTechnique::setModelNumber(const GLint& modelNumber) {
    shader->setUniform(Binder::geometry_frag::uniforms::modelNumber, modelNumber);
}*/

void DeferredGeometryTechnique::uploadProjection(const glm::mat4& projection) {
    auto& camera = getUBCamera();
    UniformBuffer::insert(camera, projection);
}

void DeferredGeometryTechnique::uploadModelUnitTables(const std::vector<ModelData*>& dataVector) {
	std::vector<ModelData::ModelUnitData> unitData(MaterialLibrary::size() * 3);
	for (auto& data : dataVector) {
		unitData[data->getMaterialIndex() * 3] = data->getModelUnitTable().diffuseUnit;
		unitData[data->getMaterialIndex() * 3 + 1] = data->getModelUnitTable().specularUnit;
		unitData[data->getMaterialIndex() * 3 + 2] = data->getModelUnitTable().normalMapUnit;
	}

	auto& modelUnitTables = getUBModelUnitTables();
	UniformBuffer::insert(modelUnitTables, unitData);
}
