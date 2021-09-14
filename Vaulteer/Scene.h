#pragma once

#include <vector>
#include <memory>

#include "Model.h"
#include "Camera.h"

class Scene
{
	friend class RenderPass;
public:
	Model* addModel(Model model);
	Camera* addCamera(Camera camera);
	void setActiveCamera(Camera* camera);
private:
	std::vector<std::unique_ptr<Model>> models;
	std::vector<std::unique_ptr<Camera>> cameras;
	Camera* activeCamera = nullptr;
};

