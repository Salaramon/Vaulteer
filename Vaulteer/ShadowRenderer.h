#pragma once

#include <vector>

#include "MyCamera.h"
#include "ShadowCubeBuffer.h"
#include "ShadowBuffer.h"
#include "ShadowCascade.h"
#include "ShadowTechnique.h"
#include "Scene.h"


class ShadowRenderer
{
public:
	const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

	unsigned int numCascades;

	ShadowRenderer(ShadowTechnique& shadowTech, MyCamera& camera, std::vector<float> cascadeBounds);

	void setCamera(MyCamera& camera);
	void updateBounds(glm::vec3 lightDirection);

	void addPointBuffer(int cubeSize);
	void addSpotBuffer(int mapSize);

	ShadowBuffer& getCascadeBuffer(int index);
	ShadowCascade& getCascade(int index);

	void render(Scene& scene);

private:
	ShadowTechnique& shadowTech;
	MyCamera& camera;

	std::vector<ShadowCubeBuffer> pointBuffers;
	std::vector<ShadowBuffer> spotBuffers;

	std::vector<ShadowBuffer> cascadeBuffers;
	std::vector<ShadowCascade> cascades;

	void drawScene(Scene& scene);
};

