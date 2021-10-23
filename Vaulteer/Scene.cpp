#include "Scene.h"

Camera* Scene::addCamera(Camera&& camera)
{
    cameras.emplace_back(std::make_unique<Camera>(camera));
	return cameras.back().get();
}

SceneLayer* Scene::addLayer(SceneLayer&& layer)
{
	sceneLayers.emplace_back(std::make_unique<SceneLayer>(std::move(layer)));
	sceneLayers.back()->parentScene = this;
	return sceneLayers.back().get();
}

void Scene::setActiveCamera(Camera* camera)
{
	activeCamera = camera;
}

Camera* Scene::getActiveCamera()
{
	return activeCamera;
}
