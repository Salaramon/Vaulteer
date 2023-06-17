#pragma once

#include <GEM.h>
#include <glad/glad.h>
#include <glfw3.h>
#include <glm/glm.hpp>

#include "OpenGL.h"

#include "Renderer/BatchManager.h"
#include "Model/Model.h"
#include "API/Camera.h"
#include "Renderer/Buffers/FrameBuffer.h"
#include "Model/Resources/ResourcePack.h"
#include "Model/Storage/Quad.h"
#include "Techniques/UniformBufferTechnique.h"
#include "Renderer/Shader.h"

#include "Scene/Scene.h"


class DeferredRenderer {
    enum GBufferTextureType {
        Position,               // vec3 position
        Normal_Material,        // vec3 normal   + int material_index
        Color_Specular,         // vec3 diffuse  + float specular
        NumTextures
    };

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
	
	inline static BatchManager batchManager;
	
	inline static std::unique_ptr<Shader> geometryShader;
	inline static std::unique_ptr<Shader> pointShader;
	inline static std::unique_ptr<Shader> dirShader;

	inline static GLint textureLibraryId;
	
public:
	inline static std::unique_ptr<Framebuffer> gbuffer;

	inline static struct RenderStats {
		size_t drawCalls = 0;
	} stats;


	static void initialize(GLint textureId, uint screenWidth, uint screenHeight) {
		textureLibraryId = textureId;

		FramebufferSpecification gbufferSpec{
			screenWidth, screenHeight,
			{{GL_RGBA16F}, {GL_RGBA16F}, {GL_RGBA16F}, {GL_DEPTH24_STENCIL8}}
		};
		gbuffer = std::make_unique<Framebuffer>(gbufferSpec);

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
		constexpr int maxPointLights = 625;
		constexpr int maxDirLights = 128;

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
		dir.setdeferred_directional_frag_dirLightData(maxDirLights);
		dir.compile();
		dirShader = std::make_unique<Shader>(
			"resources/shaders/deferred_directional_vertex.glsl", GL_VERTEX_SHADER, 
			"resources/shaders/build/deferred_directional_frag.glsl", GL_FRAGMENT_SHADER
		);
	}

	template<size_t SCENE_ID>
	static void buildLights(Scene<SCENE_ID>& scene) {

		auto pointLightView = scene.view<PointLight, ExcludeComponent<Quad>>();
		std::vector<PointLight> pointLights;
		std::vector<glm::mat4> pointLightInstanceMats;

		pointLightView.each([&](const PointLight& p) {
			pointLights.push_back(p);
			pointLightInstanceMats.push_back(p.getTransformMatrix(sphereRadius));
		});

		UniformBufferTechnique::uploadPointLightData(pointLights);
		sphereMesh->insertInstances(pointLightInstanceMats);


		auto dirLightView = scene.view<DirectionalLight, ExcludeComponent<Quad>>();
		std::vector<DirectionalLight> dirLights;

		dirLightView.each([&](const DirectionalLight& d) {
			dirLights.push_back(d);
		});

		std::vector<glm::mat4> dirLightMats(dirLights.size(), glm::mat4(1.0));
		UniformBufferTechnique::uploadDirectionalLightData(dirLights);
		quadMesh->insertInstances(dirLightMats);
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

			stats.drawCalls++;
		}

		gbuffer->unbind();
	}

	static void directionalLightPass(const CameraReference& camera) {
		dirShader->use();

		dirShader->setUniform("worldCameraPos", *camera.position);
		dirShader->setUniform("view", camera.viewMatrix());
		
		gbuffer->bindForReading();

		gbuffer->bindTextureUnit(GBufferTextureType::Position);
		dirShader->setUniform("gPosition", GBufferTextureType::Position);
		gbuffer->bindTextureUnit(GBufferTextureType::Normal_Material);
		dirShader->setUniform("gNormal", GBufferTextureType::Normal_Material);
		gbuffer->bindTextureUnit(GBufferTextureType::Color_Specular);
		dirShader->setUniform("gColor", GBufferTextureType::Color_Specular);

		quadMesh->bind();
		glDrawElementsInstanced(GL_TRIANGLES, static_cast<GLint>(quadMesh->indices.size()), GL_UNSIGNED_INT, nullptr, quadMesh->instanceCount);
		quadMesh->unbind();
		
		stats.drawCalls++;
		gbuffer->unbind();
	}
	
	static void lightingPass(const CameraReference& camera) {
		pointShader->use(); 

		auto view = Object3D::viewMatrix(*camera.position, *camera.rotation);

		pointShader->setUniform("worldCameraPos", *camera.position);
		pointShader->setUniform("view", view);

		gbuffer->bindForReading();

		gbuffer->bindTextureUnit(GBufferTextureType::Position);
		pointShader->setUniform("gPosition", GBufferTextureType::Position);
		gbuffer->bindTextureUnit(GBufferTextureType::Normal_Material);
		pointShader->setUniform("gNormal", GBufferTextureType::Normal_Material);
		gbuffer->bindTextureUnit(GBufferTextureType::Color_Specular);
		pointShader->setUniform("gColor", GBufferTextureType::Color_Specular);


		sphereMesh->bind();
		glDrawElementsInstanced(GL_TRIANGLES, static_cast<GLint>(sphereMesh->indices.size()), GL_UNSIGNED_INT, nullptr, sphereMesh->instanceCount);
		sphereMesh->unbind();

		gbuffer->unbind();
		stats.drawCalls++;
	}

	// Utility functions

	static void rebuildBatch() {
		buildBatch = true;
	}

	static void rebuildGBuffer(int width, int height) {
		gbuffer->resize(width, height);
	}
	
	static void resetStats() {
		memset(&stats, 0, sizeof(RenderStats));
	}

};
