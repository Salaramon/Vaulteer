#include "ShadowRenderer.h"

ShadowRenderer::ShadowRenderer(ShadowTechnique& shadowTech, MyCamera& camera, std::vector<float> cascadeBounds) 
	: shadowTech(shadowTech), camera(camera), numCascades(cascadeBounds.size() - 1) {
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
		//cascade.updateBounds(camera, lightDirection);
	}
}

void ShadowRenderer::addPointBuffer(int cubeSize) {
	pointBuffers.emplace_back(cubeSize);
}

void ShadowRenderer::addSpotBuffer(int mapSize) {
	spotBuffers.emplace_back(mapSize, mapSize);
}

void ShadowRenderer::render(Scene& scene) {
	GLint m_viewport[4];
	glGetIntegerv(GL_VIEWPORT, m_viewport);

	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glClear(GL_DEPTH_BUFFER_BIT);

	shadowTech.use();

	for (int i = 0; i < numCascades; i++) {
		cascadeBuffers[i].bindWrite();
		glClear(GL_DEPTH_BUFFER_BIT);

		shadowTech.setLightSpaceMatrix(cascades[i].getLightSpaceMatrix());

		drawScene(scene);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(m_viewport[0], m_viewport[1], m_viewport[2], m_viewport[3]);
}

void ShadowRenderer::drawScene(Scene& scene) {
	for (Model& model : scene) {
		model.draw(shadowTech);
	}
}

ShadowBuffer& ShadowRenderer::getCascadeBuffer(int index) {
	return cascadeBuffers[index];
}

ShadowCascade& ShadowRenderer::getCascade(int index) {
	return cascades[index];
}
