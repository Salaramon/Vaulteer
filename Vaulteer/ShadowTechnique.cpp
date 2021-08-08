#include "ShadowTechnique.h"


ShadowTechnique::ShadowTechnique(std::string pathGLSLVertexShader, std::string pathGLSLFragmentShader)
    : Technique(pathGLSLVertexShader, pathGLSLFragmentShader)
{}

void ShadowTechnique::setModel(const glm::mat4& model) {
    //shader.setUniform(Binder::shadow_vertex::uniforms::model, 1, GL_FALSE, model);
}

void ShadowTechnique::setView(const glm::mat4& view) {
    shader.setUniform(Binder::geometry_vertex::uniforms::view, 1, GL_FALSE, view);
}

void ShadowTechnique::setProjection(const glm::mat4& projection) {
    shader.setUniform(Binder::geometry_vertex::uniforms::projection, 1, GL_FALSE, projection);
}

void ShadowTechnique::setShadowMap(GLuint textureUnit) {
    shader.setUniform(Binder::shadow_frag::uniforms::shadowMap, textureUnit);
}