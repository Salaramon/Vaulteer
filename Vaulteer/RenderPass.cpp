#include "RenderPass.h"

#include "Scene.h"
#include "SceneLayer.h"
#include "Camera.h"
#include "Model.h"

RenderPass::RenderPass(Shader& shader) : shader(&shader)
{}

void RenderPass::setShader(Shader& shader)
{
	RenderPass::shader = &shader;
}

Shader* RenderPass::getShader()
{
	return shader;
}

std::vector<std::unique_ptr<SceneLayer>>& RenderPass::getSceneLayers(Scene& scene)
{
	return scene.sceneLayers;
}

std::vector<std::unique_ptr<Camera>>& RenderPass::getSceneCameras(Scene& scene)
{
	return scene.cameras;
}

std::vector<std::unique_ptr<Model>>& RenderPass::getLayerModels(SceneLayer& layer)
{
	return layer.models;
}
