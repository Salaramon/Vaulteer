#pragma once
#include <type_traits>
#include <memory>
#include <iostream>

#include "ClusterTileTechnique.h"
#include "Scene.h"

#include "Model.h"
#include "ModelData.h"
#include "LineData.h"
#include "Camera.h"

class ClusteredForwardRenderer : public ClusterTileTechnique
{
public:
	ClusteredForwardRenderer();

	//void render(Scene& scene);

private:
	glm::vec2 work_size = glm::vec2(12, 10);
	Texture2DArray computedTilingTex;
};