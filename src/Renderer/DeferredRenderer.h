#pragma once

#include <GEM.h>
#include <glad/glad.h>
#include <glfw3.h>
#include <glm/glm.hpp>

#include "OpenGL.h"

#include "Renderer/BatchManager.h"
#include "Model/Model.h"
#include "API/Camera.h"
#include "Renderer/Buffers/GBuffer.h"
#include "Model/Resources/ResourcePack.h"
#include "Techniques/UniformBufferTechnique.h"
#include "Renderer/Shader.h"

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
	inline static std::vector<glm::mat4> pointLightInstanceMats;
	
	inline static std::unique_ptr<Shader> geometryShader;
	inline static std::unique_ptr<Shader> pointShader;
	inline static std::unique_ptr<Shader> dirShader;

	inline static GLint textureLibraryId;

public:
	static void initialize(GLint textureId, uint screenWidth, uint screenHeight) {
		textureLibraryId = textureId;
		gbuffer = std::make_unique<GBuffer>(screenWidth, screenHeight);

		quadMesh = ResourceLoader::importModel("resources/quad.obj")[0];
		sphereMesh = ResourceLoader::importModel("resources/sphere-hd.obj")[0];

		coneMesh = ResourceLoader::importModel("resources/cone.obj")[0];

		//sphereRadius = 0.45f; // sphere.obj
		sphereRadius = 5.93f; // sphere-hd.obj

		coneLength = 1.0f;
		coneRadius = 1.0f;
		coneDirection = glm::vec3(.0f, 1.0f, .0f);

		loadShaders();
	}

	static void loadShaders() {
		constexpr int maxPointLights = 128;

		gem::Shader<gem::geometry_vertex> gvert;
		gvert.setgeometry_vertex_materialData(max_material_count);
		gvert.compile();
		gem::Shader<gem::geometry_frag> gf;
		gf.setgeometry_frag_materialData(max_material_count);
		gf.compile();
		geometryShader = std::make_unique<Shader>(
			"resources/shaders/build/geometry_vertex.glsl", GL_VERTEX_SHADER, 
			"resources/shaders/build/geometry_frag.glsl", GL_FRAGMENT_SHADER
		);

		gem::Shader<gem::deferred_point_vertex> dpvert;
		dpvert.compile();
		gem::Shader<gem::deferred_point_frag> dpfrag;
		dpfrag.setdeferred_point_frag_materialData(max_material_count);
		dpfrag.setdeferred_point_frag_pointLightData(maxPointLights); // TODO load from scene somehow
		dpfrag.compile();
		pointShader = std::make_unique<Shader>(
			"resources/shaders/build/deferred_point_vertex.glsl", GL_VERTEX_SHADER, 
			"resources/shaders/build/deferred_point_frag.glsl", GL_FRAGMENT_SHADER
		);
		
		gem::Shader<gem::deferred_directional_frag> dir;
		dir.setdeferred_directional_frag_materialData(max_material_count);
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
		
		auto modelView = scene.view<PropertiesModel, Meshes, Properties3D, Position3D, Rotation3D, ExcludeComponent<Transparent>>();

		if (buildBatch) {
			batchManager.batches.clear();

			modelView.each([](
				const PropertiesModel& propertiesModel, 
				const Meshes& meshes, 
				const Properties3D& properties3D, const Position3D& position3D, const Rotation3D& rotation3D) {
				batchManager.setTextureID(propertiesModel.textureID); //This should be taken from its own component in the future.

				for (Mesh* mesh : meshes) {
					batchManager.addToBatch(*mesh, Model::modelMatrix(position3D, rotation3D, properties3D));
				}
			});
			buildBatch = false;
		}
		
		if (buildLights) {
			auto lights = scene.view<PointLight>();

			lights.each([](const PointLight& p) {
				pointLights.push_back(p);
				pointLightInstanceMats.push_back(p.getTransformMatrix(sphereRadius));
			});

			UniformBufferTechnique::uploadPointLightData(pointLights);
			sphereMesh->insertInstances(pointLightInstanceMats);
		}

		
		OpenGL::depthTest(true);

		geometryPass(camera);

		OpenGL::depthTest(false);
		OpenGL::blending(true);
		OpenGL::setBlendMode(GLBlendModes::SourceAlpha, GLBlendModes::One);
		
		directionalLightPass(camera);
		OpenGL::cullFace(OpenGL::FRONT);
		
		lightingPass(camera);
		
		OpenGL::blending(false);
		OpenGL::cullFace(GL_NONE);

		buildLights = false;
	}

	static void geometryPass(const CameraReference& camera) {
		geometryShader->use();
		
		GLint texUnit = 0;
		glBindTextureUnit(texUnit, textureLibraryId);
		geometryShader->setUniform("textureLib", texUnit);

		gbuffer->clear();
		gbuffer->bindForWriting();

		geometryShader->setUniform("modelView", camera.viewMatrix()); // disregard model since everything is batched
		geometryShader->setUniform("model", glm::mat4(1.0));
		geometryShader->setUniform("normal", glm::mat4(1.0));

		for (Batch* batch : batchManager.getBatches()) {
			batch->bind();
			if (GLint texID = batch->textureID; currentlyBoundTexture != texID) {
				glBindTextureUnit(texUnit, texID);
				currentlyBoundTexture = texID;
			}

			glDrawElements(GL_TRIANGLES, batch->numIndices, GL_UNSIGNED_INT, nullptr);
			batch->unbind();
		}

		gbuffer->unbind();
	}

	static void directionalLightPass(const CameraReference& camera) {
		dirShader->use();

		dirShader->setUniform("worldCameraPos", *camera.position);
		dirShader->setUniform("view", camera.viewMatrix());

		/*auto pos = camera.position;
		auto rot = camera.rotation;
		auto view = Object3D::viewMatrix(*camera.position, *camera.rotation);*/

		if (buildLights) {
			std::vector<DirectionalLight> dirLights = {
				{{glm::vec3(1.0, 0.0, 0.0), 0.03f, 1.0f}, glm::vec3(0.0, -1.0, 0.0)}
			}; // TODO get from scene :3

			UniformBufferTechnique::uploadDirectionalLightData(dirLights);
		}
		
		/*auto pos2 = camera.position;
		auto rot2 = camera.rotation;
		auto view2 = Object3D::viewMatrix(*camera.position, *camera.rotation);*/

		gbuffer->bindForReading();

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
		pointShader->use();

		auto view = Object3D::viewMatrix(*camera.position, *camera.rotation);

		pointShader->setUniform("worldCameraPos", *camera.position);
		pointShader->setUniform("view", view);

		gbuffer->bindForReading();

		gbuffer->bindTextureUnit(GBuffer::GBufferTextureType::Position);
		pointShader->setUniform("gPosition", GBuffer::GBufferTextureType::Position);
		gbuffer->bindTextureUnit(GBuffer::GBufferTextureType::Normal_Material);
		pointShader->setUniform("gNormal", GBuffer::GBufferTextureType::Normal_Material);
		gbuffer->bindTextureUnit(GBuffer::GBufferTextureType::Color_Specular);
		pointShader->setUniform("gColor", GBuffer::GBufferTextureType::Color_Specular);


		sphereMesh->bind();
		//glDrawElementsInstanced(GL_TRIANGLES, static_cast<GLint>(sphereMesh->indices.size()), GL_UNSIGNED_INT, nullptr, sphereMesh->instanceCount);
		sphereMesh->unbind();

		gbuffer->unbind();
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
