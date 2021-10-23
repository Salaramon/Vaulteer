#pragma once

#include <vector>
#include <memory>

#include "Scene.h"

class Renderer
{
public:
	void render(Scene& scene);

	/*
	g_pLightsPipelineBack->GetRasterizerState().SetCullMode( RasterizerState::CullMode::Front );
	g_pLightsPipelineBack->GetDepthStencilState().SetDepthMode( disableDepthWrites );
	g_pLightsPipelineBack->GetBlendState().SetBlendMode( alphaBlending );
	*/

};

