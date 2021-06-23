#include "ShadowTechnique.h"


ShadowTechnique::ShadowTechnique(std::string pathGLSLVertexShader, std::string pathGLSLFragmentShader)
    : Technique(pathGLSLVertexShader, pathGLSLFragmentShader)
{}

void ShadowTechnique::setModel(const glm::mat4& model) {
    shader.setMatrix("model", model);
}

void ShadowTechnique::setView(const glm::mat4& view) {
    shader.setMatrix("view", view);
}

void ShadowTechnique::setProjection(const glm::mat4& projection) {
    shader.setMatrix("projection", projection);
}

void ShadowTechnique::setShadowMap(GLuint textureUnit) {
    shader.setInt("shadowMap", textureUnit);
}

