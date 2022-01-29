#include "DeferredGeometryTechnique.h"

void DeferredGeometryTechnique::setModelView(const glm::dmat4& model, const glm::dmat4& view) {
    glm::fmat4 modelView = view * model;
    shader->setUniform(Binder::geometry_vertex::uniforms::modelView, 1, GL_FALSE, modelView);
    setNormal(model);
}

void DeferredGeometryTechnique::setProjection(const glm::mat4& projection) {
    shader->setUniform(Binder::geometry_vertex::uniforms::projection, 1, GL_FALSE, projection);
}

void DeferredGeometryTechnique::setNormal(const glm::mat4& normal) {
    shader->setUniform(Binder::geometry_vertex::uniforms::normal, 1, GL_FALSE, glm::transpose(glm::inverse(normal)));
}

void DeferredGeometryTechnique::setTexture(const GLint& texture) {
    shader->setUniform(Binder::geometry_frag::uniforms::textureLib, texture);
}