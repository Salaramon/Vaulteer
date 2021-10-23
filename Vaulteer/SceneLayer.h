#pragma once

#include "Model.h"
#include "RenderStrata.h"
#include "Scene.h"

template<class SceneType>
class SceneLayer
{
	friend class RenderPass;
	friend class Renderer;
	friend class Scene;
public:

	SceneLayer(RenderStrata& renderStrata);
	SceneLayer(SceneLayer&& sceneLayer);
	void setRenderStrata(RenderStrata& renderStrata);
	Scene* getParentScene();
	SceneType* add(SceneType&& model);
private:
	std::vector<std::unique_ptr<SceneType>> models;
	RenderStrata* renderStrata = nullptr;
	Scene* parentScene = nullptr;
};

