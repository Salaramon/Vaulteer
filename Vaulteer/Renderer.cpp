#include "Renderer.h"

void Renderer::render(Scene& scene)
{
	for (auto& sceneLayer : scene.sceneLayers) {
		sceneLayer->renderStrata->render(*sceneLayer);
	}
}
