#include "ForwardTechnique.h"

void ForwardTechnique::setModelView(const glm::dmat4& model, const glm::dmat4& view) {
    glm::fmat4 modelView = view * model;
    shader->setUniform(Binder::forward_vertex::uniforms::modelView, 1, GL_FALSE, modelView);
    setNormal(model);
}

void ForwardTechnique::setProjection(const glm::mat4& projection)
{
	shader->setUniform(Program::forward_vertex::uniforms::projection, 1, GL_FALSE, projection);
}

void ForwardTechnique::setNormal(const glm::mat4& normal) {
    shader->setUniform(Binder::forward_vertex::uniforms::normal, 1, GL_FALSE, glm::transpose(glm::inverse(normal)));
}

void ForwardTechnique::setInverseViewMatrix(const glm::mat4& view) {
    shader->setUniform(Binder::forward_frag::uniforms::inverseViewMat, 1, GL_FALSE, glm::inverse(view));
}