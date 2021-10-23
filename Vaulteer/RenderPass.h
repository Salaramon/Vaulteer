#pragma once

#include "Shader.h"

class Scene;
class SceneLayer;
class Camera;
class Model;

class RenderPass
{
public:
	RenderPass(Shader& shader);
	virtual void render(SceneLayer& layer) = 0;
	void setShader(Shader& shader);
	Shader* getShader();

protected:
	std::vector<std::unique_ptr<SceneLayer>>& getSceneLayers(Scene& scene);
	std::vector<std::unique_ptr<Camera>>& getSceneCameras(Scene& scene);
	std::vector<std::unique_ptr<Model>>& getLayerModels(SceneLayer& layer);

	Shader* shader;
};

