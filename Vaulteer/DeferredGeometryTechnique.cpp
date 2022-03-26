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

void DeferredGeometryTechnique::setProjection(const glm::mat4& projection) {
    shader->setUniform(Binder::geometry_vertex::uniforms::projection, 1, GL_FALSE, projection);
}

void DeferredGeometryTechnique::setNormal(const glm::mat4& normal) {
    shader->setUniform(Binder::geometry_vertex::uniforms::normal, 1, GL_FALSE, glm::transpose(glm::inverse(normal)));
}

void DeferredGeometryTechnique::setTextureUnit(const GLint& texture) {
    shader->setUniform(Binder::geometry_frag::uniforms::textureLib, texture);
}

void DeferredGeometryTechnique::setModelNumber(const GLint& modelNumber) {
    shader->setUniform(Binder::geometry_frag::uniforms::modelNumber, modelNumber);
}

void DeferredGeometryTechnique::setModelUnitTable(const ModelData::ModelUnitTable& table) {
    using uni = Binder::geometry_frag::uniforms;
    for (size_t i = 0; i < 3; i++) {
        shader->setUniform(uni::unitTable[i].xDelta, table[i].xDelta);
        shader->setUniform(uni::unitTable[i].yDelta, table[i].yDelta);
        shader->setUniform(uni::unitTable[i].wDelta, table[i].wDelta);
        shader->setUniform(uni::unitTable[i].hDelta, table[i].hDelta);
        shader->setUniform(uni::unitTable[i].layerDelta, table[i].layerDelta);
    }
}
