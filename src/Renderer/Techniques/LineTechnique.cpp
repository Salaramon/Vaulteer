#include "vpch.h"
#include "Renderer/Techniques/LineTechnique.h"

void LineTechnique::setModel(glm::mat4 matrix)
{
	shader->setUniform(Program::line_vertex::uniforms::model, 1, GL_FALSE, matrix);
}

void LineTechnique::setView(glm::mat4 matrix)
{
	shader->setUniform(Program::line_vertex::uniforms::view, 1, GL_FALSE, matrix);
}

void LineTechnique::setProjection(glm::mat4 matrix)
{
	shader->setUniform(Program::line_vertex::uniforms::projection, 1, GL_FALSE, matrix);
}

void LineTechnique::setColor(glm::vec4 vector)
{
	shader->setUniform(Program::line_frag::uniforms::color, 1, vector);
}
