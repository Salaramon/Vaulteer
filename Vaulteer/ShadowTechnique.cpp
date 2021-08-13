#include "ShadowTechnique.h"


void ShadowTechnique::setModel(const glm::mat4& model) {
    //setUniform(Binder::shadow_vertex::uniforms::model, 1, GL_FALSE, model);
}

void ShadowTechnique::setView(const glm::mat4& view) {
    setUniform(Binder::geometry_vertex::uniforms::view, 1, GL_FALSE, view);
}

void ShadowTechnique::setProjection(const glm::mat4& projection) {
    setUniform(Binder::geometry_vertex::uniforms::projection, 1, GL_FALSE, projection);
}

void ShadowTechnique::setShadowMap(GLuint textureUnit) {
    setUniform(Binder::shadow_frag::uniforms::shadowMap, textureUnit);
}