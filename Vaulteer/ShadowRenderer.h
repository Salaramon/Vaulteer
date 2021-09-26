#pragma once

#include <vector>

#include "MyCamera.h"

#include "ShadowCubeBuffer.h"
#include "ShadowBuffer.h"
#include "ShadowCascade.h"
#include "ShadowTechnique.h"
#include "ShadowCubeTechnique.h"
#include "Scene.h"


class ShadowRenderer
{
public:
	const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

	unsigned int numCascades;
	unsigned int numPointBuffers;

	ShadowRenderer(ShadowTechnique& shadowTech, ShadowCubeTechnique& shadowCubeTech, MyCamera& camera, std::vector<float> cascadeBounds);

	void setCamera(MyCamera& camera);
	void updateCascadeBounds(glm::vec3 lightDirection);

	void addPointBuffer(int cubeSize, const GLSLPointLight& pointLight);
	void addSpotBuffer(int mapSize);

	ShadowBuffer& getCascadeBuffer(int index);
	ShadowCascade& getCascade(int index);
	ShadowCubeBuffer& getPointBuffer(int index);

	void render(Scene& scene);

private:
	ShadowTechnique& shadowTech;
	ShadowCubeTechnique& shadowCubeTech;
	MyCamera& camera;

	std::vector<ShadowCubeBuffer> pointBuffers;
	std::vector<ShadowBuffer> spotBuffers;

	std::vector<ShadowBuffer> cascadeBuffers;
	std::vector<ShadowCascade> cascades;

	void drawScene(Scene& scene, Technique& shader);
};

