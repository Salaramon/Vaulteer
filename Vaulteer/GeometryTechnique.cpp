#include "GeometryTechnique.h"

void GeometryTechnique::setModel(const glm::mat4& model) {
    //setUniform(Binder::geometry_vertex::uniforms::model, 1, GL_FALSE, model);

    int loc = glGetUniformLocation(getShaderID(), Binder::geometry_vertex::uniforms::model.name);
    char buff[20];
    GLsizei len = 0;
    glGetActiveUniformName(getShaderID(), loc, 20, &len, buff);
    glUniformMatrix4fv(loc, 1, GL_FALSE, &model[0][0]);
    setNormal(model);
}

void GeometryTechnique::setView(const glm::mat4& view) {
    setUniform(Binder::geometry_vertex::uniforms::view, 1, GL_FALSE, view);
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