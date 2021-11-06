#include "ForwardTechnique.h"

void ForwardTechnique::setModel(glm::mat4 matrix)
{
	shader->setUniform(Program::forward_vertex::uniforms::model, 1, GL_FALSE, matrix);
}

void ForwardTechnique::setView(glm::mat4 matrix)
{
}

void ForwardTechnique::setProjection(glm::mat4 matrix)
{
}
