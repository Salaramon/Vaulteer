#include "vpch.h"
#include "DeferredRenderer.h"

void DeferredRenderer::initialize(uint screenWidth, uint screenHeight) {
	gbuffer = std::make_unique<GBuffer>(screenWidth, screenHeight);

	quad = ResourceLoader::importModel("resources/quad.obj");
	sphereData = ResourceLoader::importModel("resources/sphere-hd.obj");
	coneData = ResourceLoader::importModel("resources/cone.obj");

	sphere = std::make_unique<Model<ModelData>>(*sphereData);
	//sphereRadius = 0.45f; // sphere.obj
	sphereRadius = 5.93f; // sphere-hd.obj

	cone = std::make_unique<Model<ModelData>>(*coneData);
	coneLength = 1.0f;
	coneRadius = 1.0f;
	coneDirection = glm::vec3(.0f, 1.0f, .0f);
}

void DeferredRenderer::preload(ResourcePack& pack) {
	auto& modelVector = pack.getAllResources();

	DeferredGeometryTechnique::uploadModelUnitTables(modelVector);
	DeferredPointLightTechnique::uploadMaterialData();
}


void DeferredRenderer::geometryPass(const Camera* camera) {
	DeferredGeometryTechnique::use();
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


	DeferredPointLightTechnique::uploadMaterialData();
}

void DeferredRenderer::directionalLightPass(const Camera* camera) {
	DeferredDirLightTechnique::use();
	glm::vec3 lightDir = glm::normalize(glm::vec3(sinf(glfwGetTime()), -1.0f, cosf(glfwGetTime())));
	
	DeferredDirLightTechnique::setWorldCameraPos(camera->getPosition());
	DeferredDirLightTechnique::setCameraViewMat(camera->getViewMatrix());

	std::vector<DirectionalLight> dirLights = {
		{{glm::vec3(1.0f,0.0f,0.0f), 0.33f, 1.0f}, lightDir}
	}; // TODO get from scene :3
	 
	DeferredDirLightTechnique::uploadDirectionalLightData(dirLights);

	gbuffer->bindForReading();

	gbuffer->bindTextureUnit(GBuffer::GBufferTextureType::Position);
	DeferredDirLightTechnique::shader->setUniform(dirFragUnis::gPosition, GBuffer::GBufferTextureType::Position);
	gbuffer->bindTextureUnit(GBuffer::GBufferTextureType::Normal_Material);
	DeferredDirLightTechnique::shader->setUniform(dirFragUnis::gNormal, GBuffer::GBufferTextureType::Normal_Material);
	gbuffer->bindTextureUnit(GBuffer::GBufferTextureType::Color_Specular);
	DeferredDirLightTechnique::shader->setUniform(dirFragUnis::gColor, GBuffer::GBufferTextureType::Color_Specular);
	
	Mesh& quadMesh = quad->getMeshes().front();
	quadMesh.bind(); 
	glDrawElements(GL_TRIANGLES, static_cast<GLint>(quadMesh.indices.size()), GL_UNSIGNED_INT, nullptr);
	quadMesh.unbind();

	gbuffer->unbind();
}

void DeferredRenderer::lightingPass(const Camera* camera) {
	DeferredPointLightTechnique::use();

	DeferredPointLightTechnique::setWorldCameraPos(camera->getPosition());
	DeferredPointLightTechnique::setView(camera->getViewMatrix());
	DeferredPointLightTechnique::setCameraViewMat(camera->getViewMatrix());
	
	if (buildLights) {
		BaseLight whiteLight = {glm::vec3(0.0f,0.0f,1.0f), 0.1f, 1.0f};
		BaseLight greenLight = {glm::vec3(0.0f,1.0f,0.0f), 0.1f, 1.0f};
		Attenuation att = { 1.0f, 0.15f, 0.042f };

		pointLights.emplace_back(att, whiteLight,  glm::vec3(20, 2, 0));
		pointLights.emplace_back(att, greenLight,  glm::vec3(0, 2, 20));

		DeferredPointLightTechnique::uploadPointLightData(pointLights);

		buildLights = false;
	}


	//DeferredLightingTechnique::setDirectionalLight(dirLight);
	//DeferredLightingTechnique::shader->setUniform(fragUnis::cascadeFarPlanes[0], 1000.0f);


	gbuffer->bindForReading();

	gbuffer->bindTextureUnit(GBuffer::GBufferTextureType::Position);
	DeferredPointLightTechnique::shader->setUniform(dirFragUnis::gPosition, GBuffer::GBufferTextureType::Position);
	gbuffer->bindTextureUnit(GBuffer::GBufferTextureType::Normal_Material);
	DeferredPointLightTechnique::shader->setUniform(dirFragUnis::gNormal, GBuffer::GBufferTextureType::Normal_Material);
	gbuffer->bindTextureUnit(GBuffer::GBufferTextureType::Color_Specular);
	DeferredPointLightTechnique::shader->setUniform(dirFragUnis::gColor, GBuffer::GBufferTextureType::Color_Specular);


	Mesh& sphereMesh = sphereData->getMeshes().front();
	sphereMesh.bind();
	int i = 0;                            
	for (auto& light : pointLights) {     
	    singlePointLightVolumePass(light, i++);
	}                                     
	//glDrawElementsInstanced(GL_TRIANGLES, static_cast<GLint>(sphereMesh.indices.size()), GL_UNSIGNED_INT, nullptr, pointLights.size());
	sphereMesh.unbind();

	gbuffer->unbind();
}

void DeferredRenderer::singlePointLightVolumePass(const PointLight& light, const int index) {
	Mesh& sphereMesh = sphereData->getMeshes().front();
	sphere->setPosition(light.position);
	sphere->setScale(glm::vec3(light.calculatePointRadius() / sphereRadius));
	DeferredPointLightTechnique::setModel(sphere->getModelMatrix());
	DeferredPointLightTechnique::setPointLightIndex(index);
	glDrawElements(GL_TRIANGLES, static_cast<GLint>(sphereMesh.indices.size()), GL_UNSIGNED_INT, nullptr);
}

/*void DeferredRenderer::singleSpotLightVolumePass(const SpotLight& light, const int index) {
	Mesh& sphereMesh = sphereData->getMeshes().front();
	sphere->setPosition(light.position);
	sphere->setScale(glm::vec3(light.calculatePointRadius() / sphereRadius));
	DeferredPointLightTechnique::setModel(sphere->getModelMatrix());
	DeferredPointLightTechnique::setPointLightIndex(index);
	glDrawElements(GL_TRIANGLES, static_cast<GLint>(sphereMesh.indices.size()), GL_UNSIGNED_INT, nullptr);
}*/

void DeferredRenderer::reloadShaders() {
	gem::Shader<gem::deferred_point_frag> point;
	point.compile();
	gem::Shader<gem::deferred_directional_frag> dir;
	dir.compile();

	DeferredGeometryTechnique::loadShader();
	DeferredDirLightTechnique::loadShader();
	DeferredPointLightTechnique::loadShader();
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
