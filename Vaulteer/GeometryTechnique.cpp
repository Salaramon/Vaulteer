#include "GeometryTechnique.h"

void GeometryTechnique::setModelView(const glm::dmat4& model, const glm::dmat4& view) {
    glm::fmat4 modelView = view * model;
    setUniform(Binder::geometry_vertex::uniforms::modelView, 1, GL_FALSE, modelView);
    setNormal(model);
}

void GeometryTechnique::setProjection(const glm::mat4& projection) {
    setUniform(Binder::geometry_vertex::uniforms::projection, 1, GL_FALSE, projection);
}

void GeometryTechnique::setNormal(const glm::mat4& model) {
    setUniform(Binder::geometry_vertex::uniforms::normal, 1, GL_FALSE, glm::transpose(glm::inverse(model)));
}

void GeometryTechnique::setTexture(const GLint& texture) {
    setUniform(Binder::geometry_frag::uniforms::diffuse1, texture);
}