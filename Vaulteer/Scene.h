#pragma once

#include <vector>
#include <memory>

#include "Camera.h"
#include "SceneLayer.h"

class Scene
{
	friend class RenderPass;
	friend class Renderer;
public:
	Camera* addCamera(Camera&& camera);
	SceneLayer* addLayer(SceneLayer&& layer);
	void setActiveCamera(Camera* camera);
	Camera* getActiveCamera();
private:
	std::vector<std::unique_ptr<SceneLayer>> sceneLayers;
	std::vector<std::unique_ptr<Camera>> cameras;
	Camera* activeCamera = nullptr;
};

