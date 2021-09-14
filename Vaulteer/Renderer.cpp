#include "Renderer.h"

void Renderer::addRenderPass(RenderPass pass)
{
	renderPasses.push_back(pass);
}

void Renderer::render(Scene& scene)
{
	for (RenderPass& pass : renderPasses) {
		pass.render(scene);
		
	}
}
