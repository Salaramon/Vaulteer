#pragma once

#include "MyCamera.h"

#include "ShadowCubeBuffer.h"
#include "ShadowBuffer.h"
#include "ShadowCascade.h"
#include "ShadowTechnique.h"
#include "ShadowCubeTechnique.h"
#include "ModelVec.h"


class ShadowRenderer
{
public:
	const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

	unsigned int numCascades;
	unsigned int numPointBuffers;
	unsigned int numSpotBuffers;

	ShadowRenderer(MyCamera& camera, std::vector<float> cascadeBounds);

	void setCamera(MyCamera& camera);
	void updateCascadeBounds(glm::vec3 lightDirection);
	void updateSpotLight(uint index, GLSLSpotLight& spotLight);

	void addPointBuffer(int cubeResolution, const GLSLPointLight& pointLight);
	void addSpotBuffer(const GLSLSpotLight& spotLight);

	ShadowBuffer& getCascadeBuffer(int index);
	ShadowCascade& getCascade(int index);
	ShadowCubeBuffer& getPointBuffer(int index);
	ShadowBuffer& getSpotBuffer(int index);

	void renderCascades(ModelVec& scene, ShadowTechnique& technique);
	void renderPointLights(ModelVec& scene, ShadowCubeTechnique& technique);
	void renderSpotLights(ModelVec& scene, ShadowTechnique& technique);

private:
	MyCamera& camera;

	std::vector<ShadowBuffer> cascadeBuffers;
	std::vector<ShadowCascade> cascades;

	std::vector<ShadowCubeBuffer> pointBuffers;
	std::vector<GLSLPointLight> pointLights;

	std::vector<ShadowBuffer> spotBuffers;
	std::vector<GLSLSpotLight> spotLights;

	void drawScene(ModelVec& scene, Technique& shader);
};

