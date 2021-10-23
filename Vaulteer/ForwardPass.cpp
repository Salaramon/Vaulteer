#include "ForwardPass.h"

void ForwardPass::render(SceneLayer& layer)
{
	shader->use();
	for (auto& model : getLayerModels(layer)) {
		glPolygonMode(model->getPolygonFaces(), model->getPolygonMode());

		shader->setUniform(Binder::forward_vertex::uniforms::view, 1, GL_FALSE, layer.getParentScene()->getActiveCamera()->getViewMatrix());
		shader->setUniform(Binder::forward_vertex::uniforms::projection, 1, GL_FALSE, layer.getParentScene()->getActiveCamera()->getProjectionMatrix());
		shader->setUniform(Binder::forward_vertex::uniforms::model, 1, GL_FALSE, model->getModelMatrix());
		model->render(*shader);
	}
}
