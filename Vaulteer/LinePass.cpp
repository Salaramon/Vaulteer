#include "LinePass.h"

void LinePass::render(SceneLayer& layer)
{
	shader->use();
	for (auto& model : getLayerModels(layer)) {
		//glPolygonMode(model->getPolygonFaces(), model->getPolygonMode());
		glLineWidth(model->getPolygonLineWidth());

		LineData* data = dynamic_cast<LineData*>(model->getData());
		shader->setUniform(Binder::line_frag::uniforms::color, 1, data->getColor());
		shader->setUniform(Binder::line_vertex::uniforms::view, 1, GL_FALSE, layer.getParentScene()->getActiveCamera()->getViewMatrix());
		shader->setUniform(Binder::line_vertex::uniforms::projection, 1, GL_FALSE, layer.getParentScene()->getActiveCamera()->getProjectionMatrix());
		shader->setUniform(Binder::line_vertex::uniforms::model, 1, GL_FALSE, model->getModelMatrix());
		model->render(*shader);
	}

}
