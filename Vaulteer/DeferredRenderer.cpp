#include "DeferredRenderer.h"

void DeferredRenderer::initialize(uint screenWidth, uint screenHeight) {
	gbuffer = std::make_unique<GBuffer>(screenWidth, screenHeight);

	ResourceLoader loader;
	quad = std::make_unique<ModelData>(std::move(loader.importModel("quad.obj")));
}

void DeferredRenderer::preload(ResourcePack& pack) {
	DeferredGeometryTechnique::setModelUnitTables(pack.getAllItems());
}

void DeferredRenderer::render(Scene& staticScene, Scene& dynamicScene) {
	const SceneObjects<Model<ModelData>>& modelVector = staticScene.getVector<Model<ModelData>>();
	const SceneObjects<Camera>& cameraVector = staticScene.getVector<Camera>();

	if (buildBatch) {
		for (auto& model : staticScene.getVector<Model<ModelData>>()){
			batchManager.setTextureID(model->getData()->getTextureID());

			for (auto& mesh : model->getData()->getMeshes()) {
				batchManager.addToBatch(mesh, model->getModelMatrix());
			}
		}

		buildBatch = false;
	}

	geometryPass(modelVector, cameraVector);
	lightingPass(modelVector, cameraVector);
}

void DeferredRenderer::geometryPass(const SceneObjects<Model<ModelData>>& modelVector, const SceneObjects<Camera>& cameraVector) {
	DeferredGeometryTechnique::shader->use();
	Camera* camera = cameraVector.front().get();

	DeferredGeometryTechnique::setProjection(camera->getProjectionMatrix());
	glm::mat4 viewMatrix = camera->getViewMatrix();

	GLint texUnit = 0;
	DeferredGeometryTechnique::setTextureUnit(texUnit);
	
	gbuffer.get()->bindForWriting();
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT); // glClearNamedFramebufferfv()

	/*
	for (auto& model : modelVector) {
		ModelData* modelData = model->getData();
		const std::vector<Mesh>& modelDataMeshes = modelData->getMeshes();

		for (const Mesh& mesh : modelDataMeshes) {
			mesh.vertexArray.bind();
			//glDrawElementsInstanced(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, 0, instances.size());
			//debug("Drawing mesh: " + std::to_string(mesh.getObjectKey()) + "\n", "glDrawElements");
			glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, 0);
			mesh.vertexArray.unbind();
		}
	}
	*/

	DeferredGeometryTechnique::setModelView(glm::mat4(1.0), viewMatrix);

	for (Batch& batch : batchManager.getBatches()) {
		batch.bind();
		GLint texID = batch.textureID;
		if (currentlyBoundTexture != texID) {
			glBindTextureUnit(texUnit, 1);
			currentlyBoundTexture = texID;
		}

		glDrawElements(GL_TRIANGLES, batch.numIndices, GL_UNSIGNED_INT, 0);
		batch.unbind();
	}

	gbuffer.get()->unbind();
}

void DeferredRenderer::lightingPass(const SceneObjects<Model<ModelData>>& modelVector, const SceneObjects<Camera>& cameraVector) {

	DeferredLightingTechnique::shader->use();
	Camera* camera = cameraVector.front().get();

	glm::vec3 lightDir = glm::normalize(glm::vec3(sinf(glfwGetTime()), -1.0f, cosf(glfwGetTime())));
	
	DirectionalLight dirLight = { glm::vec3(1.0f), 0.03f, 1.0f, lightDir }; // TODO get from scene :3


	DeferredLightingTechnique::setWorldCameraPos(camera->getPosition());
	DeferredLightingTechnique::setCameraViewMat(camera->getViewMatrix());
	DeferredLightingTechnique::setDirectionalLight(dirLight);

	DeferredLightingTechnique::setMaterialSpecularIntensity(1.0f); // TODO - ah the classic deferred shading material lookup circumvented
	DeferredLightingTechnique::setMaterialShininess(32.0f);         // in the cleanest possible way

	DeferredLightingTechnique::shader->setUniform(fragUnis::cascadeFarPlanes[0], 1000.0f);


	gbuffer.get()->bindForReading();

	int texId = 0;
	glBindTextureUnit(texId, gbuffer->textures[texId]);
	DeferredLightingTechnique::shader->setUniform(fragUnis::gPosition, texId++);
	glBindTextureUnit(texId, gbuffer->textures[texId]);
	DeferredLightingTechnique::shader->setUniform(fragUnis::gNormal, texId++);
	glBindTextureUnit(texId, gbuffer->textures[texId]);
	DeferredLightingTechnique::shader->setUniform(fragUnis::gColor, texId++);

	Mesh& quadMesh = quad->getMeshes().front();
	quadMesh.bind();
	glDrawElements(GL_TRIANGLES, quadMesh.indices.size(), GL_UNSIGNED_INT, 0);
	quadMesh.unbind();

	gbuffer.get()->unbind();
}

void DeferredRenderer::reloadShaders() {
	DeferredGeometryTechnique::reloadShader();
	DeferredLightingTechnique::reloadShader();
}

void DeferredRenderer::rebuildBatch() {
	buildBatch = true;
}
