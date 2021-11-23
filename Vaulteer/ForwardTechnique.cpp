#include "ForwardTechnique.h"

void ForwardTechnique::setModel(glm::mat4 matrix)
{
	shader->setUniform(Program::forward_vertex::uniforms::model, 1, GL_FALSE, matrix);
}

void ForwardTechnique::setView(glm::mat4 matrix)
{
	shader->setUniform(Program::forward_vertex::uniforms::view, 1, GL_FALSE, matrix);
}

void ForwardTechnique::setProjection(glm::mat4 matrix)
{
	shader->setUniform(Program::forward_vertex::uniforms::projection, 1, GL_FALSE, matrix);
}
