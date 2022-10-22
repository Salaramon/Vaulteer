#include "vpch.h"
#include "DeferredRenderer.h"

void DeferredRenderer::initialize(uint screenWidth, uint screenHeight) {
	gbuffer = std::make_unique<GBuffer>(screenWidth, screenHeight);

	quad = ResourceLoader::importModel("resources/quad.obj");
}

void DeferredRenderer::preload(ResourcePack& pack) {
	auto& modelVector = pack.getAllResources();

	DeferredGeometryTechnique::uploadModelUnitTables(modelVector);
	DeferredLightingTechnique::uploadMaterialData(modelVector);
}


void DeferredRenderer::geometryPass(const Camera* camera) {
	OpenGL::enableDepthTest();
	DeferredGeometryTechnique::shader->use();
	DeferredGeometryTechnique::uploadProjection(camera->getProjectionMatrix());
	glm::mat4 viewMatrix = camera->getViewMatrix();

	GLint texUnit = 0;
	DeferredGeometryTechnique::setTextureUnit(texUnit);

	gbuffer->clear();
	gbuffer->bindForWriting();

	DeferredGeometryTechnique::setModelView(glm::mat4(1.0), viewMatrix);

	for (Batch& batch : BatchManager::getBatches(batchManager)) {
		batch.bind();
		if (GLint texID = batch.textureID; currentlyBoundTexture != texID) {
			glBindTextureUnit(texUnit, texID);
			currentlyBoundTexture = texID;
		}

		glDrawElements(GL_TRIANGLES, batch.numIndices, GL_UNSIGNED_INT, nullptr);
		batch.unbind();
	}

	gbuffer->unbind();
}

void DeferredRenderer::lightingPass(const Camera* camera) {
	OpenGL::disableDepthTest();
	DeferredLightingTechnique::shader->use();
	glm::vec3 lightDir = glm::normalize(glm::vec3(sinf(glfwGetTime()), -1.0f, cosf(glfwGetTime())));

	DirectionalLight dirLight = {{glm::vec3(1.0f), 0.03f, 1.0f}, lightDir}; // TODO get from scene :3


	DeferredLightingTechnique::setWorldCameraPos(camera->getPosition());
	DeferredLightingTechnique::setCameraViewMat(camera->getViewMatrix());
	DeferredLightingTechnique::setDirectionalLight(dirLight);

	//DeferredLightingTechnique::shader->setUniform(fragUnis::cascadeFarPlanes[0], 1000.0f);


	gbuffer->bindForReading();

	gbuffer->bindTextureUnit(GBuffer::GBufferTextureType::Position);
	DeferredLightingTechnique::shader->setUniform(fragUnis::gPosition, GBuffer::GBufferTextureType::Position);
	gbuffer->bindTextureUnit(GBuffer::GBufferTextureType::Normal_Material);
	DeferredLightingTechnique::shader->setUniform(fragUnis::gNormal, GBuffer::GBufferTextureType::Normal_Material);
	gbuffer->bindTextureUnit(GBuffer::GBufferTextureType::Color_Specular);
	DeferredLightingTechnique::shader->setUniform(fragUnis::gColor, GBuffer::GBufferTextureType::Color_Specular);

	Mesh& quadMesh = quad->getMeshes().front();
	quadMesh.bind();
	glDrawElements(GL_TRIANGLES, static_cast<GLint>(quadMesh.indices.size()), GL_UNSIGNED_INT, 0);
	quadMesh.unbind();

	gbuffer->unbind();
}

void DeferredRenderer::reloadShaders() {
	DeferredGeometryTechnique::reloadShader();
	DeferredLightingTechnique::reloadShader();
}

void DeferredRenderer::rebuildBatch() {
	buildBatch = true;
}

void DeferredRenderer::rebuildGBuffer(int width, int height) {
	gbuffer.reset();
	gbuffer = std::make_unique<GBuffer>(width, height);
}

void DeferredRenderer::copyGBufferDepth(GLint fbo) {
	gbuffer->bindForReading();
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);

	GLuint width = gbuffer->width, height = gbuffer->height;
	glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT, GL_NEAREST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
