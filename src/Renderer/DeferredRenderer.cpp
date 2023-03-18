#include "vpch.h"
#include "DeferredRenderer.h"

void DeferredRenderer::initialize(uint screenWidth, uint screenHeight) {
	gbuffer = std::make_unique<GBuffer>(screenWidth, screenHeight);
}

void DeferredRenderer::preload(ResourcePack& pack) {
	//auto& modelVector = pack.getAllResources();

	//DeferredGeometryTechnique::uploadModelUnitTables(modelVector);
	DeferredPointLightTechnique::uploadMaterialData();
}


void DeferredRenderer::geometryPass(const Camera* camera) {
	//DeferredGeometryTechnique::use();
	//DeferredGeometryTechnique::uploadProjection(camera->getProjectionMatrix());
	//glm::mat4 viewMatrix = camera->getViewMatrix();

	GLint texUnit = 0;
	DeferredGeometryTechnique::setTextureUnit(texUnit);

	gbuffer->clear();
	gbuffer->bindForWriting();

	//DeferredGeometryTechnique::setModelView(glm::mat4(1.0), viewMatrix);

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

void DeferredRenderer::directionalLightPass(const Camera* camera) {
	//DeferredDirLightTechnique::use();
	glm::vec3 lightDir = glm::normalize(glm::vec3(sinf(glfwGetTime()), -1.0f, cosf(glfwGetTime())));
	
	//DeferredDirLightTechnique::setWorldCameraPos(camera->getPosition());
	//DeferredDirLightTechnique::setCameraViewMat(camera->getViewMatrix());

	if (buildLights) {
		std::vector<DirectionalLight> dirLights = {
			{{glm::vec3(1.0f), 0.03f, 1.0f}, lightDir}
		}; // TODO get from scene :3
		 
		DeferredDirLightTechnique::uploadDirectionalLightData(dirLights);
	}

	gbuffer->bindForReading();

	gbuffer->bindTextureUnit(GBuffer::GBufferTextureType::Position);
	//DeferredDirLightTechnique::shader->setUniform(fragUnis::gPosition, GBuffer::GBufferTextureType::Position);
	gbuffer->bindTextureUnit(GBuffer::GBufferTextureType::Normal_Material);
	//DeferredDirLightTechnique::shader->setUniform(fragUnis::gNormal, GBuffer::GBufferTextureType::Normal_Material);
	gbuffer->bindTextureUnit(GBuffer::GBufferTextureType::Color_Specular);
	//DeferredDirLightTechnique::shader->setUniform(fragUnis::gColor, GBuffer::GBufferTextureType::Color_Specular);
	
	quadMesh->bind();
	glDrawElements(GL_TRIANGLES, static_cast<GLint>(quadMesh->indices.size()), GL_UNSIGNED_INT, nullptr);
	quadMesh->unbind();

	gbuffer->unbind();
}

void DeferredRenderer::lightingPass(const Camera* camera) {
	//DeferredPointLightTechnique::use();

	/*
	DeferredPointLightTechnique::setWorldCameraPos(camera->getPosition());
	DeferredPointLightTechnique::setView(camera->getViewMatrix());
	DeferredPointLightTechnique::setCameraViewMat(camera->getViewMatrix());
	*/
	
	if (buildLights) {
		BaseLight whiteLight = {glm::vec3(1.0f), 0.1f, 1.0f };
		Attenuation att = { 1.0f, 0.15f, 0.042f };

		pointLights.emplace_back(att, whiteLight,  glm::vec3(20, 2, 0));
		pointLights.emplace_back(att, whiteLight,  glm::vec3(0, 2, 20));

		DeferredPointLightTechnique::uploadPointLightData(pointLights);

		buildLights = false;
	}


	//DeferredLightingTechnique::setDirectionalLight(dirLight);
	//DeferredLightingTechnique:://shader->setUniform(fragUnis::cascadeFarPlanes[0], 1000.0f);


	gbuffer->bindForReading();

	gbuffer->bindTextureUnit(GBuffer::GBufferTextureType::Position);
	//DeferredPointLightTechnique::shader->setUniform(fragUnis::gPosition, GBuffer::GBufferTextureType::Position);
	gbuffer->bindTextureUnit(GBuffer::GBufferTextureType::Normal_Material);
	//DeferredPointLightTechnique::shader->setUniform(fragUnis::gNormal, GBuffer::GBufferTextureType::Normal_Material);
	gbuffer->bindTextureUnit(GBuffer::GBufferTextureType::Color_Specular);
	//DeferredPointLightTechnique::shader->setUniform(fragUnis::gColor, GBuffer::GBufferTextureType::Color_Specular);


	sphereMesh->bind();
	int i = 0;
	for (auto& light : pointLights) {
		singleLightVolumePass(light, i++);
	}
	sphereMesh->unbind();

	gbuffer->unbind();
}

void DeferredRenderer::singleLightVolumePass(const PointLight& light, const int index) {
	Position3D position(light.position);
	Rotation3D rotation(glm::vec3(1.f));
	Properties3D properties(Properties3D{ 
		.axisLockDirection = glm::vec3(1.f), 
		.isAxisLocked =  false, 
		.scale = glm::vec3(light.calculatePointRadius() / 5.93f) });

	Object3DUtility object3DUtility(position, rotation, properties);

	DeferredPointLightTechnique::setModel(object3DUtility.modelMatrix());
	DeferredPointLightTechnique::setPointLightIndex(index);
	glDrawElements(GL_TRIANGLES, static_cast<GLint>(sphereMesh->indices.size()), GL_UNSIGNED_INT, nullptr);
}

void DeferredRenderer::reloadShaders() {
	//DeferredGeometryTechnique::reloadShader();
	//DeferredPointLightTechnique::reloadShader();
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
