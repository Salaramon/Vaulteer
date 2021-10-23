#include "SceneLayer.h"

#include "Scene.h"

SceneLayer::SceneLayer(RenderStrata& renderStrata)
{
	setRenderStrata(renderStrata);
}

SceneLayer::SceneLayer(SceneLayer&& sceneLayer) :
	models(std::move(sceneLayer.models)),
	renderStrata(std::move(sceneLayer.renderStrata))
{}

void SceneLayer::setRenderStrata(RenderStrata& renderStrata)
{
	SceneLayer::renderStrata = &renderStrata;
}

Scene* SceneLayer::getParentScene()
{
	return parentScene;
}

Model* SceneLayer::addModel(Model&& model)
{
	models.push_back(std::make_unique<Model>(std::move(model)));
	return models.back().get();
}