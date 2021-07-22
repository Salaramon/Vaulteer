#include "GeometryTechnique.h"

GeometryTechnique::GeometryTechnique(std::string pathGLSLVertexCode, std::string pathGLSLFragmentCode)
    : Technique(pathGLSLVertexCode, pathGLSLFragmentCode)
{
}

void GeometryTechnique::setModel(const glm::mat4& model) {
    shader.setMatrix("model", model);
    setNormal(model);
}

void GeometryTechnique::setView(const glm::mat4& view) {
    shader.setMatrix("view", view);
}

void GeometryTechnique::setProjection(const glm::mat4& projection) {
    shader.setMatrix("projection", projection);
}

void GeometryTechnique::setNormal(const glm::mat4& model) {
    shader.setMatrix("normal", glm::transpose(glm::inverse(model)));
}

void GeometryTechnique::setTexture(const GLuint& texture) {
    shader.setInt("texture_1", texture);
}