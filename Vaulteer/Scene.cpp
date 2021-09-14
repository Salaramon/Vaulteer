#include "Scene.h"

Model* Scene::addModel(Model model)
{
	models.push_back(std::make_unique<Model>(model));
	return models.back().get();
}

Camera* Scene::addCamera(Camera camera)
{
    cameras.push_back(std::make_unique<Camera>(camera));
	return cameras.back().get();;
}

void Scene::setActiveCamera(Camera* camera)
{
	activeCamera = camera;
}
