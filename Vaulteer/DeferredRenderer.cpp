#include "DeferredRenderer.h"

void DeferredRenderer::initialize(uint screenWidth, uint screenHeight) {
	gbuffer = std::make_unique<GBuffer>(screenWidth, screenHeight);

	ResourceLoader loader;
	quad = std::make_unique<ModelData>(std::move(loader.importModel("quad.obj")));
}

void DeferredRenderer::render(Scene& scene) {
	const SceneObjects<Model<ModelData>>& modelVector = scene.getVector<Model<ModelData>>();
	const SceneObjects<Camera>& cameraVector = scene.getVector<Camera>();

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

	for (auto& model : modelVector) {
		DeferredGeometryTechnique::setModelView(model->getModelMatrix(), viewMatrix);

		ModelData* modelData = model->getData();
		const std::vector<Mesh>& modelDataMeshes = modelData->getMeshes();
		glBindTextureUnit(texUnit, modelData->getTextureID());

		for (const Mesh& mesh : modelDataMeshes) {
			mesh.vertexArray.bind();
			//glDrawElementsInstanced(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, 0, instances.size());
			//debug("Drawing mesh: " + std::to_string(mesh.getObjectKey()) + "\n", "glDrawElements");
			glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, 0);
			mesh.vertexArray.unbind();
		}
	}

	gbuffer.get()->unbind();
}

void DeferredRenderer::lightingPass(const SceneObjects<Model<ModelData>>& modelVector, const SceneObjects<Camera>& cameraVector) {

	DeferredLightingTechnique::shader->use();
	Camera* camera = cameraVector.front().get();

	glm::vec3 lightDir = glm::vec3(0.0f, -1.0f, 0.0f);
	DirectionalLight dirLight = { glm::vec3(1.0f), 0.01f, 0.3f, lightDir }; // TODO get from scene :3


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

	const Mesh& quadMesh = quad->getMeshes().front();
	quadMesh.vertexArray.bind();
	glDrawElements(GL_TRIANGLES, quadMesh.indices.size(), GL_UNSIGNED_INT, 0);
	quadMesh.vertexArray.unbind();

	gbuffer.get()->unbind();
}
