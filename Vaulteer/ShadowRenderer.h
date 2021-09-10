#pragma once

#include <vector>
#include <glad/glad.h>

#include "MyCamera.h"
#include "ShadowBuffer.h"
#include "ShadowCascade.h"


class ShadowRenderer
{
public:
	const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

	ShadowRenderer(MyCamera& camera, std::vector<float> cascadeBounds);

	void setCamera(MyCamera& camera);
	void updateBounds(glm::vec3 lightDirection);

	void addBuffer(ShadowBuffer& buffer);
	void addCascade(ShadowCascade& cascade);

	ShadowBuffer& getBuffer(int index);
	ShadowCascade& getCascade(int index);

	// TODO this will be used eventually - need structure for scene and all assorted objects though...
	//void render(Scene& scene);

	unsigned int numCascades;

private:
	MyCamera& camera;
	std::vector<ShadowBuffer> buffers;
	std::vector<ShadowCascade> cascades;
};

