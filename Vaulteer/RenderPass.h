#pragma once

#include "Scene.h"

class RenderPass
{
public:
	RenderPass(Shader& shader);
	void render(Scene& scene);
	/*
	g_pLightsPipelineBack->GetRasterizerState().SetCullMode( RasterizerState::CullMode::Front );
    g_pLightsPipelineBack->GetDepthStencilState().SetDepthMode( disableDepthWrites );
    g_pLightsPipelineBack->GetBlendState().SetBlendMode( alphaBlending );
	*/
	
private:
	Shader* shader;
};

