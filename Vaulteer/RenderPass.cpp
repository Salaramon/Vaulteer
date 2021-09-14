#include "RenderPass.h"

RenderPass::RenderPass(Shader& shader) : shader(&shader)
{}

void RenderPass::render(Scene& scene)
{
	shader->use();
	for (auto& model : scene.models) {
		shader->setUniform(Binder::lightsource_vertex::uniforms::view, 1, GL_FALSE, scene.activeCamera->getViewMatrix());
		shader->setUniform(Binder::lightsource_vertex::uniforms::projection, 1, GL_FALSE, scene.activeCamera->getProjectionMatrix());
		shader->setUniform(Binder::lightsource_vertex::uniforms::model, 1, GL_FALSE, model->getModelMatrix());
		model->render(*shader);
	}
}
