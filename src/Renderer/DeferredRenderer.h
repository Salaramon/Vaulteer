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

	
constexpr int maxPointLights = 625;
constexpr int maxDirLights = 128;

constexpr int initialShadowMapCount = 32;

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
	inline static std::unique_ptr<Shader> pointShadowShader;
	inline static std::unique_ptr<Shader> dirShadowShader;

	inline static GLint textureLibraryId;
	
	inline static std::vector<glm::vec3> lightDirections;
	inline static std::vector<glm::vec3> lightPositions;

public:
	inline static std::unique_ptr<Framebuffer> gbuffer;
	inline static std::unique_ptr<Framebuffer> shadowBuffer;

	inline static bool drawShadowVolumes = false;

	inline static struct RenderStats {
		size_t drawCalls = 0;
	} stats;


	static void initialize(GLint textureId, int screenWidth, int screenHeight) {
		textureLibraryId = textureId;

		quadMesh = ResourceLoader::importModel("resources/quad.obj")[0];
		sphereMesh = ResourceLoader::importModel("resources/sphere-hd.obj")[0];

		coneMesh = ResourceLoader::importModel("resources/cone.obj")[0];

		//sphereRadius = 0.45f; // sphere.obj
		sphereRadius = 5.93f; // sphere-hd.obj

		coneLength = 1.0f;
		coneRadius = 1.0f;
		coneDirection = glm::vec3(.0f, 1.0f, .0f);
		
		initFramebuffers(screenWidth, screenHeight);

		loadShaders();
	}

	static void initFramebuffers(int screenWidth, int screenHeight) {
		FramebufferSpecification gbufferSpec{
			screenWidth, screenHeight,
			{{GL_RGBA16F}, {GL_RGBA16F}, {GL_RGBA16F}, {GL_DEPTH24_STENCIL8}}
		};
		gbuffer = std::make_unique<Framebuffer>(gbufferSpec);

		FramebufferSpecification shadowbufferSpec {
			screenWidth, screenHeight,
			{{.textureFormat = GL_DEPTH24_STENCIL8, .depthStencilMode = GL_STENCIL_INDEX}},
			GL_TEXTURE_2D_ARRAY, initialShadowMapCount
		};
		shadowBuffer = std::make_unique<Framebuffer>(shadowbufferSpec);
	}

	static void loadShaders() {
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

		pointShadowShader = std::make_unique<Shader>(
			"resources/shaders/volume_vertex.glsl", GL_VERTEX_SHADER,
			"resources/shaders/volume_geom.glsl", GL_GEOMETRY_SHADER,
			"resources/shaders/volume_frag.glsl", GL_FRAGMENT_SHADER
		);
		dirShadowShader = std::make_unique<Shader>(
			"resources/shaders/volume_vertex.glsl", GL_VERTEX_SHADER,
			"resources/shaders/volume_dir_geom.glsl", GL_GEOMETRY_SHADER,
			"resources/shaders/volume_frag.glsl", GL_FRAGMENT_SHADER
		);
	}

	template<size_t SCENE_ID>
	static void buildLights(Scene<SCENE_ID>& scene) {
		// -- dirlights --

		auto dirLightView = scene.view<DirectionalLight, ExcludeComponent<Quad>>();
		std::vector<DirectionalLight> dirLights;

		dirLightView.each([&](const DirectionalLight& d) {
			dirLights.push_back(d);
			lightDirections.push_back(d.direction);
		});

		std::vector<glm::mat4> dirLightMats(dirLights.size(), glm::mat4(1.0));
		UniformBufferTechnique::uploadDirectionalLightData(dirLights);
		quadMesh->insertInstances(dirLightMats);
		
		// -- pointlights --

		auto pointLightView = scene.view<PointLight, ExcludeComponent<Quad>>();
		std::vector<PointLight> pointLights;
		std::vector<glm::mat4> pointLightInstanceMats;

		pointLightView.each([&](const PointLight& p) {
			pointLights.push_back(p);
			pointLightInstanceMats.push_back(p.getTransformMatrix(sphereRadius));
			lightPositions.push_back(p.position);
		});

		UniformBufferTechnique::uploadPointLightData(pointLights);
		sphereMesh->insertInstances(pointLightInstanceMats);

		int totalLights = lightDirections.size() + lightPositions.size();
		if (totalLights > shadowBuffer->specification.layers) {
			shadowBuffer->growDepthLayers(totalLights);
		}
	}

	template<size_t SCENE_ID>
	static void render(Scene<SCENE_ID>& scene) {
		auto camera = scene.getActiveCamera();

		auto bshBoundrary = [](glm::vec4 sphere) { return true; }; //Utility function combined with necessary rendering logic need to be applied here.
		
		if (buildBatch) {
			batchManager.batches.clear();

			auto batchView = scene.view<PropertiesModel, Meshes, Properties3D, Position3D, Rotation3D, Opaque, ExcludeComponent<Instanced>>();
			batchView.each([](const PropertiesModel& propertiesModel, const Meshes& meshes,
							  const Properties3D& properties3D, const Position3D& position3D, const Rotation3D& rotation3D,
							  const Opaque&) {
				batchManager.setTextureID(propertiesModel.textureID); //This should be taken from its own component in the future.

				for (Mesh* mesh : meshes) {
					batchManager.addToBatch(*mesh, Model::modelMatrix(position3D, rotation3D, properties3D));
				}
			});
			buildBatch = false;
		}

		OpenGL::depthTest(true);

		geometryPass(scene);

		OpenGL::stencilTest(true);

		shadowVolumePass(scene);

		OpenGL::stencilTest(false);

		OpenGL::depthTest(false);
		OpenGL::blending(true);
		OpenGL::setBlendMode(GLBlendModes::SourceAlpha, GLBlendModes::One);
		
		directionalLightPass(camera);

		OpenGL::cullFace(OpenGL::FRONT);
		
		pointLightPass(camera);
		
		OpenGL::blending(false);
		OpenGL::cullFace(GL_NONE);
	}
	
	template<size_t SCENE_ID>
	static void geometryPass(Scene<SCENE_ID>& scene) {
		geometryShader->use();
		
		GLint texUnit = 0;
		glBindTextureUnit(texUnit, textureLibraryId);
		geometryShader->setUniform("textureLib", texUnit);
		
		gbuffer->clear();
		gbuffer->bindForWriting();
		
		//geometryShader->setUniform("model", glm::mat4(1.0)); // disregard model since everything is batched
		geometryShader->setUniform("normal", glm::mat4(1.0));

		for (Batch* batch : batchManager.getBatches()) {
			batch->bind();
			if (GLint texID = batch->textureID; currentlyBoundTexture != texID) {
				glBindTextureUnit(texUnit, texID);
				currentlyBoundTexture = texID;
			}

			glDrawElements(GL_TRIANGLES, batch->numIndices, GL_UNSIGNED_INT, nullptr);

			stats.drawCalls++;
		}
		Batch::unbind();
		
		auto modelView = scene.view<PropertiesModel, Meshes, Position3D, Rotation3D, Properties3D, Instanced, Opaque>();
		modelView.each([](const PropertiesModel& propertiesModel, const Meshes& meshes, 
						  const Position3D& position, const Rotation3D& rotation, const Properties3D& properties3D,
			              const Instanced&, const Opaque&) {
			for (Mesh* mesh : meshes) {
				mesh->bind();
				glDrawElementsInstanced(mesh->getType(), mesh->getNumIndices(), GL_UNSIGNED_INT, nullptr, mesh->instanceCount);
				stats.drawCalls++;
			}
		});

		gbuffer->unbind();
	}


	template<size_t SCENE_ID>
	static void shadowVolumePass(Scene<SCENE_ID>& scene) {

		auto camera = scene.getActiveCamera();
		auto view = camera.viewMatrix();

		glStencilFunc(GL_ALWAYS, 0, 0xFF);  // Set all stencil values to 0

		// set stencil test according to zfail algorithm
		glStencilOpSeparate(GL_BACK, GL_KEEP, GL_INCR_WRAP, GL_KEEP);
		glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_DECR_WRAP, GL_KEEP);
			
		// Clamp depth values at infinity to max depth. Required for back cap of volume to be included
		glEnable(GL_DEPTH_CLAMP);

		// do not render to depth or color buffer
		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
		glDepthMask(GL_FALSE);

		auto shadowModelView = scene.view<PropertiesModel, Meshes, Position3D, Rotation3D, Properties3D, Shadow, Opaque>();
		dirShadowShader->use();
		
		gbuffer->bindForReading();
		shadowBuffer->bindForWriting();

		for (int i = 0; i < lightDirections.size(); i++) {
			shadowBuffer->bindDepthLayer(i);
			shadowBuffer->clearDepthStencil();
			Framebuffer::copyDepth(*gbuffer, *shadowBuffer);
			
			glm::vec4 lightDir = view * glm::vec4(lightDirections[i], 0.0f);
			dirShadowShader->setUniform("lightDir_viewSpace", glm::vec3(lightDir.x, lightDir.y, lightDir.z));
			
			shadowModelView.each([](const PropertiesModel&, const Meshes& meshes, 
							        const Position3D& position, const Rotation3D& rotation, const Properties3D& properties3D, 
							        const Shadow&, const Opaque&) {
				for (Mesh* mesh : meshes) {
					KYSE_ASSERT(mesh->getType() == GL_TRIANGLES_ADJACENCY);
					mesh->bind();
					glDrawElementsInstanced(mesh->getType(), mesh->getNumIndices(), GL_UNSIGNED_INT, nullptr, mesh->instanceCount);
					stats.drawCalls++;
				}
				Mesh::unbind();
			});
		}

		pointShadowShader->use();
		int numDirLights = lightDirections.size();

		for (int i = 0; i < lightPositions.size(); i++) {
			shadowBuffer->bindDepthLayer(i + numDirLights);
			shadowBuffer->clearDepthStencil();
			Framebuffer::copyDepth(*gbuffer, *shadowBuffer);

			glm::vec4 lightViewPos = view * glm::vec4(lightPositions[i], 1.0f);
			pointShadowShader->setUniform("lightPos_viewSpace", glm::vec3(lightViewPos.x, lightViewPos.y, lightViewPos.z));
			
			shadowModelView.each([](const PropertiesModel&, const Meshes& meshes, 
							        const Position3D& position, const Rotation3D& rotation, const Properties3D& properties3D, 
							        const Shadow&, const Opaque&) {
				for (Mesh* mesh : meshes) {
					KYSE_ASSERT(mesh->getType() == GL_TRIANGLES_ADJACENCY);
					mesh->bind();
					glDrawElementsInstanced(mesh->getType(), mesh->getNumIndices(), GL_UNSIGNED_INT, nullptr, mesh->instanceCount);
					stats.drawCalls++;
				}
				Mesh::unbind();
			});
		}
		
		shadowBuffer->unbind();
		
		// reset to working state
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
		glDepthMask(GL_TRUE);
		
		glDisable(GL_DEPTH_CLAMP);

		if (drawShadowVolumes) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

			OpenGL::depthTest(false);

			if (!lightDirections.empty()) {
				dirShadowShader->use();
				dirShadowShader->setUniform("lightDir", lightDirections[0]);

				shadowModelView.each([](const PropertiesModel&, const Meshes& meshes, 
								        const Position3D& position, const Rotation3D& rotation, const Properties3D& properties3D, 
								        const Shadow&, const Opaque&) {

					for (Mesh* mesh : meshes) {
						KYSE_ASSERT(mesh->getType() == GL_TRIANGLES_ADJACENCY);
						mesh->bind();
						glDrawElementsInstanced(mesh->getType(), mesh->getNumIndices(), GL_UNSIGNED_INT, nullptr, mesh->instanceCount);
						stats.drawCalls++;
					}
					Mesh::unbind();
				});
			}

			if (!lightPositions.empty()) {
				pointShadowShader->use();
				glm::vec4 lightViewPos = view * glm::vec4(lightPositions[0], 1.0f);
				pointShadowShader->setUniform("lightPos", glm::vec3(lightViewPos.x, lightViewPos.y, lightViewPos.z));

				shadowModelView.each([](const PropertiesModel&, const Meshes& meshes, 
								        const Position3D& position, const Rotation3D& rotation, const Properties3D& properties3D, 
								        const Shadow&, const Opaque&) {

					for (Mesh* mesh : meshes) {
						KYSE_ASSERT(mesh->getType() == GL_TRIANGLES_ADJACENCY);
						mesh->bind();
						glDrawElementsInstanced(mesh->getType(), mesh->getNumIndices(), GL_UNSIGNED_INT, nullptr, mesh->instanceCount);
						stats.drawCalls++;
					}
					Mesh::unbind();
				});
			}

			OpenGL::depthTest(true);

			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
	}

	static void directionalLightPass(const CameraReference& camera) {
		dirShader->use();

		dirShader->setUniform("worldCameraPos", *camera.position);
		
		gbuffer->bindForReading();

		gbuffer->bindColorTextureUnit(GBufferTextureType::Position);
		dirShader->setUniform("gPosition", GBufferTextureType::Position);
		gbuffer->bindColorTextureUnit(GBufferTextureType::Normal_Material);
		dirShader->setUniform("gNormal", GBufferTextureType::Normal_Material);
		gbuffer->bindColorTextureUnit(GBufferTextureType::Color_Specular);
		dirShader->setUniform("gColor", GBufferTextureType::Color_Specular);

		glBindTextureUnit(3, shadowBuffer->depthTexture);
		dirShader->setUniform("shadowTexture", 3);

		quadMesh->bind();
		glDrawElementsInstanced(quadMesh->getType(), quadMesh->getNumIndices(), GL_UNSIGNED_INT, nullptr, quadMesh->instanceCount);
		Mesh::unbind();
		
		stats.drawCalls++;
		gbuffer->unbind();
	}
	
	static void pointLightPass(const CameraReference& camera) {
		pointShader->use(); 

		pointShader->setUniform("worldCameraPos", *camera.position);

		gbuffer->bindForReading();

		gbuffer->bindColorTextureUnit(GBufferTextureType::Position);
		pointShader->setUniform("gPosition", GBufferTextureType::Position);
		gbuffer->bindColorTextureUnit(GBufferTextureType::Normal_Material);
		pointShader->setUniform("gNormal", GBufferTextureType::Normal_Material);
		gbuffer->bindColorTextureUnit(GBufferTextureType::Color_Specular);
		pointShader->setUniform("gColor", GBufferTextureType::Color_Specular);

		glBindTextureUnit(3, shadowBuffer->depthTexture);
		pointShader->setUniform("shadowTexture", 3);

		uint numDirLights = lightDirections.size();
		pointShader->setUniform("dirLightCount", numDirLights);

		sphereMesh->bind();
		glDrawElementsInstanced(sphereMesh->getType(), sphereMesh->getNumIndices(), GL_UNSIGNED_INT, nullptr, sphereMesh->instanceCount);
		Mesh::unbind();

		gbuffer->unbind();
		stats.drawCalls++;
	}

	// Utility functions

	static void rebuildBatch() {
		buildBatch = true;
	}

	static void resizeFramebuffers(int width, int height) {
		gbuffer->resize(width, height);
		shadowBuffer->resize(width, height);
	}
	
	static void resetStats() {
		memset(&stats, 0, sizeof(RenderStats));
	}

};
