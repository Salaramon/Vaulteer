#pragma once
#include <type_traits>
#include <memory>
#include <iostream>

#include "Renderer/Techniques/ClusterTileTechnique.h"
#include "Scene/Scene.h"

#include "Model/Model.h"
#include "Model/Data/ModelData.h"
#include "Model/Data/LineData.h"
#include "API/Camera.h"

class ClusteredForwardRenderer : public ClusterTileTechnique
{
public:
	ClusteredForwardRenderer();

	//void render(Scene& scene);

private:
	glm::vec2 work_size = glm::vec2(12, 10);
	Texture2DArray computedTilingTex;
};