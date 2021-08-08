#include "GeometryTechnique.h"

GeometryTechnique::GeometryTechnique(std::string pathGLSLVertexCode, std::string pathGLSLFragmentCode)
    : Technique(pathGLSLVertexCode, pathGLSLFragmentCode)
{
}

void GeometryTechnique::setModel(const glm::mat4& model) {
    //shader.setUniform(Binder::geometry_vertex::uniforms::model, 1, GL_FALSE, model);

    int loc = glGetUniformLocation(shader.getShaderID(), Binder::geometry_vertex::uniforms::model.name);
    char buff[20];
    GLsizei len = 0;
    glGetActiveUniformName(shader.getShaderID(), loc, 20, &len, buff);
    glUniformMatrix4fv(loc, 1, GL_FALSE, &model[0][0]);
    setNormal(model);
}

void GeometryTechnique::setView(const glm::mat4& view) {
    shader.setUniform(Binder::geometry_vertex::uniforms::view, 1, GL_FALSE, view);
}

void GeometryTechnique::setProjection(const glm::mat4& projection) {
    shader.setUniform(Binder::geometry_vertex::uniforms::projection, 1, GL_FALSE, projection);
}

void GeometryTechnique::setNormal(const glm::mat4& model) {
    shader.setUniform(Binder::geometry_vertex::uniforms::normal, 1, GL_FALSE, glm::transpose(glm::inverse(model)));
}

void GeometryTechnique::setTexture(const GLint& texture) {
    shader.setUniform(Binder::geometry_frag::uniforms::diffuse1, texture);
}