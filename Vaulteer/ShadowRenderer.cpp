#include "ShadowRenderer.h"

ShadowRenderer::ShadowRenderer(ShadowTechnique& shadowTech, ShadowCubeTechnique& shadowCubeTech, MyCamera& camera, std::vector<float> cascadeBounds)
	: shadowTech(shadowTech), shadowCubeTech(shadowCubeTech), camera(camera), numCascades(cascadeBounds.size() - 1), numPointBuffers(0) {
	for (int i = 0; i < numCascades; i++) {
		cascades.emplace_back(cascadeBounds[i], cascadeBounds[i + (size_t)1]);
		cascadeBuffers.emplace_back(SHADOW_WIDTH, SHADOW_HEIGHT);
	}
}

void ShadowRenderer::setCamera(MyCamera& camera) {
	this->camera = camera;
}

void ShadowRenderer::updateCascadeBounds(glm::vec3 lightDirection) {
	for (ShadowCascade& cascade : cascades) {
		cascade.updateBounds(camera, lightDirection);
	}
}

void ShadowRenderer::render(Scene& scene) {
	// save current viewport for restoration later
	GLint m_viewport[4];
	glGetIntegerv(GL_VIEWPORT, m_viewport);

	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);

	shadowTech.use();

	for (int i = 0; i < cascades.size(); i++) {
		cascadeBuffers[i].bindWrite();
		glClear(GL_DEPTH_BUFFER_BIT);

		shadowTech.setLightSpaceMatrix(cascades[i].getLightSpaceMatrix());

		drawScene(scene, shadowTech);
	}

	shadowCubeTech.use();

	for (int i = 0; i < numPointBuffers; i++) {
		ShadowCubeBuffer& buffer = pointBuffers[i];
		glViewport(0, 0, buffer.cubeSize, buffer.cubeSize);

		buffer.bindWrite();
		glClear(GL_DEPTH_BUFFER_BIT);

		// TODO: separate pointlight data from buffer object (like ShadowCascade)?
		glm::mat4 modelMat(1.0f);
		modelMat = glm::translate(modelMat, buffer.lightPos);

		shadowCubeTech.setModel(modelMat);
		shadowCubeTech.setFarPlane(buffer.farPlane);
		shadowCubeTech.setLightPos(buffer.lightPos);
		shadowCubeTech.setShadowMatrices(buffer.getShadowTransforms());

		drawScene(scene, shadowCubeTech);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glViewport(m_viewport[0], m_viewport[1], m_viewport[2], m_viewport[3]);
}

void ShadowRenderer::drawScene(Scene& scene, Technique& shader) {
	for (Model& model : scene) {
		model.draw(shader);
	}
}

ShadowBuffer& ShadowRenderer::getCascadeBuffer(int index) {
	return cascadeBuffers[index];
}

ShadowCascade& ShadowRenderer::getCascade(int index) {
	return cascades[index];
}

ShadowCubeBuffer& ShadowRenderer::getPointBuffer(int index) {
	return pointBuffers[index];
}

void ShadowRenderer::addPointBuffer(int cubeSize, const GLSLPointLight& pointLight) {
	pointBuffers.emplace_back(cubeSize, pointLight);
	numPointBuffers++;
}

void ShadowRenderer::addSpotBuffer(int mapSize) {
	spotBuffers.emplace_back(mapSize, mapSize);
}
