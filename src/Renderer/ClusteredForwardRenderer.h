#pragma once

#include "Model/Model.h"
#include "API/Camera.h"

class ClusteredForwardRenderer
{
public:
	ClusteredForwardRenderer();

	//void render(Scene& scene);

private:
	glm::vec2 work_size = glm::vec2(12, 10);
	Texture2DArray computedTilingTex;
};