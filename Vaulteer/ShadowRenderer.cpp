#include "ShadowRenderer.h"

ShadowRenderer::ShadowRenderer(MyCamera& camera, std::vector<float> cascadeBounds) : camera(camera), numCascades(cascadeBounds.size() - 1) {
	for (int i = 0; i < numCascades; i++) {
		cascades.emplace_back(cascadeBounds[i], cascadeBounds[i + 1]);
		cascadeBuffers.emplace_back(SHADOW_WIDTH, SHADOW_HEIGHT);
	}
}

void ShadowRenderer::setCamera(MyCamera& camera) {
	this->camera = camera;
}

void ShadowRenderer::updateBounds(glm::vec3 lightDirection) {
	for (ShadowCascade& cascade : cascades) {
		cascade.updateBounds(camera, lightDirection);
	}
}

ShadowBuffer& ShadowRenderer::getCascadeBuffer(int index) {
	return cascadeBuffers[index];
}

ShadowCascade& ShadowRenderer::getCascade(int index) {
	return cascades[index];
}
