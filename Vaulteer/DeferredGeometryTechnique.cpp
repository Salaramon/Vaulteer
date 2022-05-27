#include "vpch.h"
#include "DeferredGeometryTechnique.h"

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
    camera.insert(projection);
}

void DeferredGeometryTechnique::uploadModelUnitTables(const std::vector<ModelData*>& dataVector) {
    std::vector<ModelData::ModelUnitData> unitData;
    for (auto& data : dataVector) {
        unitData.push_back(data->getModelUnitTable().diffuseUnit);
        unitData.push_back(data->getModelUnitTable().specularUnit);
        unitData.push_back(data->getModelUnitTable().normalMapUnit);
    }

    modelUnitTables.insert(unitData);
}
