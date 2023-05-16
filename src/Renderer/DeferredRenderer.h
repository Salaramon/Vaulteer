#pragma once

#include <GEM.h>

#include "Renderer/Techniques/DeferredGeometryTechnique.h"
#include "Renderer/Techniques/DeferredPointLightTechnique.h"
#include "Renderer/Techniques/DeferredDirLightTechnique.h"
#include "Renderer/Techniques/BlendingTechnique.h"

#include "OpenGL.h"

#include "Renderer/BatchManager.h"
#include "Model/Model.h"
#include "API/Camera.h"
#include "Renderer/Buffers/GBuffer.h"
#include "Model/Resources/ResourcePack.h"
#include "Techniques/UniformBufferTechnique.h"


class DeferredRenderer : 
	public DeferredGeometryTechnique, 
	public DeferredPointLightTechnique {
private:

	inline static std::unique_ptr<GBuffer> gbuffer;

	inline static Mesh* quadMesh;

	// sphere used to align affected light area with pointlight radius
	inline static float sphereRadius;
	inline static Mesh* sphereMesh;
	
	inline static float coneLength;
	inline static float coneRadius;
	inline static glm::vec3 coneDirection;
	inline static Mesh* coneMesh;

	inline static GLint currentlyBoundTexture = -1;
	inline static bool buildBatch = true;

	// for debugging while we're not fetching lights from scene
	inline static bool buildLights = true;

	inline static BatchManager batchManager;

	// TODO
	inline static std::vector<PointLight> pointLights;

public:
	static void initialize(uint screenWidth, uint screenHeight) {
		gbuffer = std::make_unique<GBuffer>(screenWidth, screenHeight);

		quadMesh = ResourceLoader::importModel("resources/quad.obj")[0];
		sphereMesh = ResourceLoader::importModel("resources/sphere-hd.obj")[0];
		coneMesh = ResourceLoader::importModel("resources/cone.obj")[0];

		//sphere = std::make_unique<Model<ModelData>>(*sphereData);
		//sphereRadius = 0.45f; // sphere.obj
		sphereRadius = 5.93f; // sphere-hd.obj

		//cone = std::make_unique<Model<ModelData>>(*coneData);
		coneLength = 1.0f;
		coneRadius = 1.0f;
		coneDirection = glm::vec3(.0f, 1.0f, .0f);
	}

	static void reloadShaders() {
		/*
			Will be replaced by a runtime binder functionality
		*/
		gem::Shader<gem::deferred_point_frag> point;
		point.compile();
		gem::Shader<gem::deferred_directional_frag> dir;
		dir.compile();

		//DeferredGeometryTechnique::loadShader();
		//DeferredDirLightTechnique::loadShader();
		//DeferredPointLightTechnique::loadShader();
	}

	template<size_t SCENE_ID>
	static void render(Scene<SCENE_ID>& scene) {
		auto camera = scene.getActiveCamera();

		auto bshBoundrary = [](glm::vec4 sphere) { return true; }; //Utility function combined with necessary rendering logic need to be applied here.
		
		auto modelView = scene.view<PropertiesModel, Meshes, Properties3D, Position3D, Rotation3D>();

		if (buildBatch) {
			modelView.each([](const PropertiesModel& propertiesModel, const Meshes& meshes, const Properties3D& properties3D, const Position3D& position3D, const Rotation3D& rotation3D) {
				
				BatchManager::setTextureID(batchManager, propertiesModel.textureID); //This should be taken from its own component in the future.

				for (Mesh* mesh : meshes) {
					BatchManager::addToBatch(batchManager, *mesh, Model::modelMatrix(position3D, rotation3D, properties3D));
				}
				
			});
			buildBatch = false;
		}

		
		OpenGL::enableDepthTest();

		geometryPass(camera);

		OpenGL::disableDepthTest();
		OpenGL::enableBlending();
		OpenGL::setBlendMode(GLBlendModes::SourceAlpha, GLBlendModes::One);

		directionalLightPass(camera);

		OpenGL::enableCullFace(OpenGL::FRONT);

		lightingPass(camera);

		OpenGL::disableBlending();
		OpenGL::disableCullFace();
	}

	static void geometryPass(const CameraReference& camera) {
		/*
			Will be replaced by a runtime binder functionality
		*/
		DeferredGeometryTechnique::shader().use();

		glm::mat4 viewMatrix = camera.viewMatrix();

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

	static void directionalLightPass(const CameraReference& camera) {
		/*
			Will be replaced by a runtime binder functionality
		*/
		DeferredDirLightTechnique::shader().use();
		glm::vec3 lightDir = glm::normalize(glm::vec3(sinf(glfwGetTime()), -1.0f, cosf(glfwGetTime())));

	
		DeferredDirLightTechnique::setWorldCameraPos(*camera.position);
		DeferredDirLightTechnique::setCameraViewMat(camera.viewMatrix());

		if (buildLights) {
			std::vector<DirectionalLight> dirLights = {
				{{glm::vec3(1.0f), 0.03f, 1.0f}, lightDir}
			}; // TODO get from scene :3

			UniformBufferTechnique::uploadDirectionalLightData(dirLights);
		}

		gbuffer->bindForReading();

		/*
			Will be replaced by a runtime binder functionality
		*/
		gbuffer->bindTextureUnit(GBuffer::GBufferTextureType::Position);
		DeferredDirLightTechnique::shader().setUniform(fragUnis::gPosition, GBuffer::GBufferTextureType::Position);
		gbuffer->bindTextureUnit(GBuffer::GBufferTextureType::Normal_Material);
		DeferredDirLightTechnique::shader().setUniform(fragUnis::gNormal, GBuffer::GBufferTextureType::Normal_Material);
		gbuffer->bindTextureUnit(GBuffer::GBufferTextureType::Color_Specular);
		DeferredDirLightTechnique::shader().setUniform(fragUnis::gColor, GBuffer::GBufferTextureType::Color_Specular);

		quadMesh->bind();
		glDrawElements(GL_TRIANGLES, static_cast<GLint>(quadMesh->indices.size()), GL_UNSIGNED_INT, nullptr);
		quadMesh->unbind();

		gbuffer->unbind();
	}

	static void lightingPass(const CameraReference& camera) {
		/*
			Will be replaced by a runtime binder functionality
		*/
		DeferredPointLightTechnique::shader().use();

		glm::mat4 viewMatrix = camera.viewMatrix();

		DeferredPointLightTechnique::setWorldCameraPos(*camera.position);
		DeferredPointLightTechnique::setView(viewMatrix);
		DeferredPointLightTechnique::setCameraViewMat(viewMatrix);

		if (buildLights) {
			BaseLight whiteLight = { glm::vec3(1.0f), 0.1f, 1.0f };
			Attenuation att = { 1.0f, 0.15f, 0.042f };

			pointLights.emplace_back(att, whiteLight, glm::vec3(20, 2, 0));
			pointLights.emplace_back(att, whiteLight, glm::vec3(0, 2, 20));

			UniformBufferTechnique::uploadPointLightData(pointLights);

			buildLights = false;
		}

		gbuffer->bindForReading();

		gbuffer->bindTextureUnit(GBuffer::GBufferTextureType::Position);
		DeferredPointLightTechnique::shader().setUniform(fragUnis::gPosition, GBuffer::GBufferTextureType::Position);
		gbuffer->bindTextureUnit(GBuffer::GBufferTextureType::Normal_Material);
		DeferredPointLightTechnique::shader().setUniform(fragUnis::gNormal, GBuffer::GBufferTextureType::Normal_Material);
		gbuffer->bindTextureUnit(GBuffer::GBufferTextureType::Color_Specular);
		DeferredPointLightTechnique::shader().setUniform(fragUnis::gColor, GBuffer::GBufferTextureType::Color_Specular);


		sphereMesh->bind();
		int i = 0;
		for (auto& light : pointLights) {
			singleLightVolumePass(light, i++);
		}
		//glDrawElementsInstanced(GL_TRIANGLES, static_cast<GLint>(sphereMesh->indices.size()), GL_UNSIGNED_INT, nullptr, pointLights.size());
		sphereMesh->unbind();

		gbuffer->unbind();
	}

	static void singleLightVolumePass(const PointLight& light, const int index) {
		Position3D position(light.position);
		Rotation3D rotation(glm::vec3(1.f));
		Properties3D properties {
			.scale = glm::vec3(light.calculatePointRadius() / sphereRadius),
			.axisLockDirection = glm::vec3(1.f),
			.isAxisLocked = false
		};

		DeferredPointLightTechnique::setModel(Model::modelMatrix(position, rotation, properties));
		DeferredPointLightTechnique::setPointLightIndex(index);
		glDrawElements(GL_TRIANGLES, static_cast<GLint>(sphereMesh->indices.size()), GL_UNSIGNED_INT, nullptr);
	}

	// Utility functions

	static void rebuildBatch() {
		buildBatch = true;
	}

	static void rebuildGBuffer(int width, int height) {
		gbuffer.reset();
		gbuffer = std::make_unique<GBuffer>(width, height);
	}

	static void copyGBufferDepth(GLint fbo) {
		gbuffer->bindForReading();
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);

		GLuint width = gbuffer->width, height = gbuffer->height;
		glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT, GL_NEAREST);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

};
