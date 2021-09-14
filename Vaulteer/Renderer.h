#pragma once

#include <vector>

#include "RenderPass.h"

class Renderer
{
public:
	void addRenderPass(RenderPass pass);
	void render(Scene& scene);
private:
	std::vector<RenderPass> renderPasses;
};

