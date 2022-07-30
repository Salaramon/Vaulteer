#include "vpch.h"
#include "DeferredRenderer.h"

void DeferredRenderer::initialize(uint screenWidth, uint screenHeight) {
	gbuffer = std::make_unique<GBuffer>(screenWidth, screenHeight);

	ResourceLoader loader;
	quad = std::make_unique<ModelData>(std::move(loader.importModel("resources/quad.obj")));
}

void DeferredRenderer::preload(ResourcePack& pack) {
	auto& modelVector = pack.getAllResources();

	DeferredGeometryTechnique::uploadModelUnitTables(modelVector);
	DeferredLightingTechnique::uploadMaterialData(modelVector);
}


void DeferredRenderer::geometryPass(Camera* camera) {
	OpenGL::enableStencilTest();
	//glStencilMask(0xFF); // don't need to change this
	glStencilFunc(GL_ALWAYS, 0, 0xFF);
	glStencilOp(GL_ZERO, GL_INCR, GL_INCR);

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
		GLint texID = batch.textureID;
		if (currentlyBoundTexture != texID) {
			glBindTextureUnit(texUnit, texID);
			currentlyBoundTexture = texID;
		}

		glDrawElements(GL_TRIANGLES, batch.numIndices, GL_UNSIGNED_INT, 0);
		batch.unbind();
	}

	gbuffer->unbind();

	OpenGL::disableStencilTest();
}

void DeferredRenderer::lightingPass(Camera* camera) {
	OpenGL::disableDepthTest();
	DeferredLightingTechnique::shader->use();
	glm::vec3 lightDir = glm::normalize(glm::vec3(sinf(glfwGetTime()), -1.0f, cosf(glfwGetTime())));
	
	DirectionalLight dirLight = { glm::vec3(1.0f), 0.03f, 1.0f, lightDir }; // TODO get from scene :3


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
	glDrawElements(GL_TRIANGLES, quadMesh.indices.size(), GL_UNSIGNED_INT, 0);
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
	glBlitFramebuffer(0, 0, gbuffer->width, gbuffer->height, 0, 0, gbuffer->width, gbuffer->height, GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT, GL_NEAREST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
