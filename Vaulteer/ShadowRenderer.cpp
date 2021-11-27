#include "ShadowRenderer.h"

ShadowRenderer::ShadowRenderer(MyCamera& camera, std::vector<float> cascadeBounds)
		: camera(camera), numCascades(cascadeBounds.size() - 1), numPointBuffers(0), numSpotBuffers(0) {
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

void ShadowRenderer::updateSpotLight(uint index, GLSLSpotLight& spotLight) {
	spotLights[index] = spotLight;
}

void ShadowRenderer::renderCascades(ModelVec& scene, ShadowTechnique& technique) {
	// save current viewport for restoration later
	GLint m_viewport[4];
	glGetIntegerv(GL_VIEWPORT, m_viewport);

	technique.use();

	for (int i = 0; i < cascades.size(); i++) {
		ShadowBuffer& buffer = cascadeBuffers[i];
		glViewport(0, 0, buffer.width, buffer.height);

		buffer.bindWrite();
		glClear(GL_DEPTH_BUFFER_BIT);

		technique.setLightSpaceMatrix(cascades[i].getLightSpaceMatrix());

		drawScene(scene, technique);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(m_viewport[0], m_viewport[1], m_viewport[2], m_viewport[3]);
}

void ShadowRenderer::renderPointLights(ModelVec& scene, ShadowCubeTechnique& technique) {
	// save current viewport for restoration later
	GLint m_viewport[4];
	glGetIntegerv(GL_VIEWPORT, m_viewport);

	technique.use();

	for (int i = 0; i < numPointBuffers; i++) {
		ShadowCubeBuffer& buffer = pointBuffers[i];
		glViewport(0, 0, buffer.cubeSize, buffer.cubeSize);

		buffer.bindWrite();
		glClear(GL_DEPTH_BUFFER_BIT);

		// TODO: separate pointlight data from buffer object (like ShadowCascade)?
		glm::mat4 modelMat(1.0f);
		modelMat = glm::translate(modelMat, buffer.lightPos);

		technique.setModel(modelMat);
		technique.setFarPlane(buffer.farPlane);
		technique.setLightPos(buffer.lightPos);
		technique.setShadowMatrices(buffer.getShadowTransforms());

		drawScene(scene, technique);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(m_viewport[0], m_viewport[1], m_viewport[2], m_viewport[3]);
}

void ShadowRenderer::renderSpotLights(ModelVec& scene, ShadowTechnique& technique) {
	// save current viewport for restoration later
	GLint m_viewport[4];
	glGetIntegerv(GL_VIEWPORT, m_viewport);

	technique.use();

	for (int i = 0; i < numSpotBuffers; i++) {
		ShadowBuffer& buffer = spotBuffers[i];
		glViewport(0, 0, buffer.width, buffer.height);

		buffer.bindWrite();
		glClear(GL_DEPTH_BUFFER_BIT);

		GLSLSpotLight& spotLight = spotLights[i];

		glm::mat4 modelMat(1.0f);
		modelMat = glm::translate(modelMat, spotLight.position);

		technique.setModel(modelMat);
		technique.setLightSpaceMatrix(GLSLSpotLight::getLightSpaceMatrix(spotLight));

		drawScene(scene, technique);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(m_viewport[0], m_viewport[1], m_viewport[2], m_viewport[3]);
}

void ShadowRenderer::drawScene(ModelVec& scene, Technique& shader) {
	for (Model& model : scene) {
		shader.setModel(model.getModelMat());
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

ShadowBuffer& ShadowRenderer::getSpotBuffer(int index) {
	return spotBuffers[index];
}

void ShadowRenderer::addPointBuffer(int cubeResolution, const GLSLPointLight& pointLight) {
	pointBuffers.emplace_back(cubeResolution, pointLight);
	numPointBuffers++;
}

void ShadowRenderer::addSpotBuffer(const GLSLSpotLight& spotLight) {
	spotBuffers.emplace_back(spotLight);
	spotLights.emplace_back(spotLight);
	numSpotBuffers++;
}
