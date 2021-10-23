#pragma once

#include "RenderPass.h"
#include "Scene.h"

class LinePass : public RenderPass
{
public:
	using RenderPass::RenderPass;

	void render(SceneLayer& layer) override;
};

