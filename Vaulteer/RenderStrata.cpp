#include "RenderStrata.h"
#include "Scene.h"

void RenderStrata::render(SceneLayer& layer)
{
	for (auto& pass : renderPasses) {
		pass->render(layer);
	}
}
