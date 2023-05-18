#pragma once

#include <GEM.h>

#include "OpenGL.h"

#include "Renderer/BatchManager.h"
#include "Model/Model.h"
#include "API/Camera.h"
#include "Renderer/Buffers/GBuffer.h"
#include "Model/Resources/ResourcePack.h"
#include "Techniques/UniformBufferTechnique.h"

#include "Scene/Scene.h"


class DeferredRenderer {
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
	
	inline static std::unique_ptr<Shader> geometryShader;
	inline static std::unique_ptr<Shader> pointShader;
	inline static std::unique_ptr<Shader> dirShader;

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

		loadShaders();
	}

	static void loadShaders() {
		constexpr int maxMaterials = 128;

		gem::Shader<gem::geometry_vertex> gvert;
		gvert.compile();
		geometryShader = std::make_unique<Shader>(
			"resources/shaders/build/geometry_vertex.glsl", GL_VERTEX_SHADER, 
			"resources/shaders/geometry_frag.glsl", GL_FRAGMENT_SHADER
		);

		gem::Shader<gem::deferred_point_vertex> dpvert;
		dpvert.compile();
		gem::Shader<gem::deferred_point_frag> dpfrag;
		dpfrag.setdeferred_point_frag_materialData(maxMaterials);
		dpfrag.setdeferred_point_frag_pointLightData(2); // TODO load from scene somehow
		dpfrag.compile();
		pointShader = std::make_unique<Shader>(
			"resources/shaders/build/deferred_point_vertex.glsl", GL_VERTEX_SHADER, 
			"resources/shaders/build/deferred_point_frag.glsl", GL_FRAGMENT_SHADER
		);
		
		gem::Shader<gem::deferred_directional_frag> dir;
		dir.setdeferred_directional_frag_materialData(maxMaterials);
		dir.compile();
		dirShader = std::make_unique<Shader>(
			"resources/shaders/deferred_directional_vertex.glsl", GL_VERTEX_SHADER, 
			"resources/shaders/build/deferred_directional_frag.glsl", GL_FRAGMENT_SHADER
		);
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
		geometryShader->use();

		glm::mat4 viewMatrix = camera.viewMatrix();

		GLint texUnit = 0;
		geometryShader->setUniform("textureLib", texUnit);

		gbuffer->clear();
		gbuffer->bindForWriting();

		//TODO clean up model?
		geometryShader->setUniform("modelView", viewMatrix); // disregard model since everything is batched
		geometryShader->setUniform("model", glm::mat4(1.0));
		geometryShader->setUniform("normal", glm::mat4(1.0));

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
		dirShader->use();
		glm::vec3 lightDir = glm::normalize(glm::vec3(sinf(glfwGetTime()), -1.0f, cosf(glfwGetTime())));

	
		dirShader->setUniform("worldCameraPos", *camera.position);
		dirShader->setUniform("view", camera.viewMatrix());  // TODO removed cameraViewMat in frag, see if this works for both vert/frag shaders

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
		dirShader->setUniform("gPosition", GBuffer::GBufferTextureType::Position);
		gbuffer->bindTextureUnit(GBuffer::GBufferTextureType::Normal_Material);
		dirShader->setUniform("gNormal", GBuffer::GBufferTextureType::Normal_Material);
		gbuffer->bindTextureUnit(GBuffer::GBufferTextureType::Color_Specular);
		dirShader->setUniform("gColor", GBuffer::GBufferTextureType::Color_Specular);

		quadMesh->bind();
		glDrawElements(GL_TRIANGLES, static_cast<GLint>(quadMesh->indices.size()), GL_UNSIGNED_INT, nullptr);
		quadMesh->unbind();

		gbuffer->unbind();
	}

	static void lightingPass(const CameraReference& camera) {
		/*
			Will be replaced by a runtime binder functionality
		*/
		pointShader->use();

		glm::mat4 viewMatrix = camera.viewMatrix();

		pointShader->setUniform("worldCameraPos", *camera.position);
		pointShader->setUniform("view", camera.viewMatrix()); // TODO removed cameraViewMat in frag, see if this works for both vert/frag shaders

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
		pointShader->setUniform("gPosition", GBuffer::GBufferTextureType::Position);
		gbuffer->bindTextureUnit(GBuffer::GBufferTextureType::Normal_Material);
		pointShader->setUniform("gNormal", GBuffer::GBufferTextureType::Normal_Material);
		gbuffer->bindTextureUnit(GBuffer::GBufferTextureType::Color_Specular);
		pointShader->setUniform("gColor", GBuffer::GBufferTextureType::Color_Specular);


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

		pointShader->setUniform("model", Model::modelMatrix(position, rotation, properties));
		pointShader->setUniform("pointLightIndex", index);
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
