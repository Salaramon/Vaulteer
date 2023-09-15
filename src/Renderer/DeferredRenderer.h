#pragma once

#include <GEM.h>
#include <glad/glad.h>
#include <glfw3.h>
#include <glm/glm.hpp>

#include "Batch.h"
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

constexpr int initialShadowMapCount = 2;

constexpr size_t default_vertex_buffer_size = 4000000 / sizeof(VertexImpl);
constexpr size_t default_index_buffer_size = 1000000 / sizeof(GLuint);
	

namespace DeferredRenderer {
    enum GBufferTextureType {
        Position,               // vec3 position
        Normal_Material,        // vec3 normal   + int material_index
        Color_Specular,         // vec3 diffuse  + float specular
        NumTextures
    };

	inline struct DeferredRendererData {
		std::unique_ptr<Shader> geometryShader;
		std::unique_ptr<Shader> pointShader;
		std::unique_ptr<Shader> dirShader;
		std::unique_ptr<Shader> pointShadowShader;
		std::unique_ptr<Shader> dirShadowShader;

		std::unique_ptr<CameraReference> renderCamera;

		std::unique_ptr<Mesh> quadMesh;
		std::unique_ptr<Mesh> sphereMesh; // sphere used to align affected light area with pointlight radius
		float sphereRadius;

		/* float coneLength; float coneRadius; glm::vec3 coneDirection;
		std::unique_ptr<Mesh> coneMesh;*/

		GLint currentlyBoundTexture = -1;

		std::vector<std::unique_ptr<Batch>> batches;

		std::vector<Model*>* shadowModels;
		std::vector<glm::vec3> dirLights;
		std::vector<glm::vec3> pointLights;

		GLint textureLibraryID;

		std::unique_ptr<Framebuffer> gbuffer;
		std::unique_ptr<Framebuffer> shadowBuffer;
	} data;

	inline struct RenderStats {
		size_t drawCalls = 0;
	} stats;


	// controls
	inline bool buildBatch = false; // TODO
	inline bool drawShadowVolumes = false;

	
	inline void createBatch(size_t vbSize = default_vertex_buffer_size, size_t ibSize = default_index_buffer_size) {
		data.batches.push_back(std::make_unique<Batch>(data.textureLibraryID, vbSize, ibSize));
	}

	inline void addToBatch(const Mesh& mesh, glm::mat4 modelMat) {
		if (data.batches.empty()) {
			createBatch();
		}

		if (Batch* batch = data.batches.back().get(); !batch->add(mesh, modelMat)) {
			createBatch();
			data.batches.back()->add(mesh, modelMat);
		}
	}


	inline void initFramebuffers(int screenWidth, int screenHeight) {
		FramebufferSpecification gbufferSpec{
			screenWidth, screenHeight,
			{{GL_RGBA16F}, {GL_RGBA16F}, {GL_RGBA16F}, {GL_DEPTH24_STENCIL8}}
		};
		data.gbuffer = std::make_unique<Framebuffer>(gbufferSpec);

		FramebufferSpecification shadowbufferSpec {
			screenWidth, screenHeight,
			{{.textureFormat = GL_DEPTH24_STENCIL8, .depthStencilMode = GL_STENCIL_INDEX}},
			GL_TEXTURE_2D_ARRAY, initialShadowMapCount
		};
		data.shadowBuffer = std::make_unique<Framebuffer>(shadowbufferSpec);
	}

	inline void loadShaders() {
		gem::Shader<gem::geometry_vertex> gvert;
		gvert.setgeometry_vertex_materialData(max_material_count);
		gvert.compile();
		gem::Shader<gem::geometry_frag> gf;
		gf.setgeometry_frag_materialData(max_material_count);
		gf.compile();
		data.geometryShader = std::make_unique<Shader>(
			"resources/shaders/build/geometry_vertex.glsl", GL_VERTEX_SHADER, 
			"resources/shaders/build/geometry_frag.glsl", GL_FRAGMENT_SHADER
		);

		gem::Shader<gem::deferred_point_vertex> dpvert;
		dpvert.compile();
		gem::Shader<gem::deferred_point_frag> dpfrag;
		dpfrag.setdeferred_point_frag_materialData(max_material_count);
		dpfrag.setdeferred_point_frag_pointLightData(maxPointLights); // TODO load from scene somehow
		dpfrag.compile();
		data.pointShader = std::make_unique<Shader>(
			"resources/shaders/build/deferred_point_vertex.glsl", GL_VERTEX_SHADER, 
			"resources/shaders/build/deferred_point_frag.glsl", GL_FRAGMENT_SHADER
		);
		
		gem::Shader<gem::deferred_directional_frag> dir;
		dir.setdeferred_directional_frag_materialData(max_material_count);
		dir.setdeferred_directional_frag_dirLightData(maxDirLights);
		dir.compile();
		data.dirShader = std::make_unique<Shader>(
			"resources/shaders/deferred_directional_vertex.glsl", GL_VERTEX_SHADER, 
			"resources/shaders/build/deferred_directional_frag.glsl", GL_FRAGMENT_SHADER
		);

		data.pointShadowShader = std::make_unique<Shader>(
			"resources/shaders/volume_vertex.glsl", GL_VERTEX_SHADER,
			"resources/shaders/volume_geom.glsl", GL_GEOMETRY_SHADER,
			"resources/shaders/volume_frag.glsl", GL_FRAGMENT_SHADER
		);
		data.dirShadowShader = std::make_unique<Shader>(
			"resources/shaders/volume_vertex.glsl", GL_VERTEX_SHADER,
			"resources/shaders/volume_dir_geom.glsl", GL_GEOMETRY_SHADER,
			"resources/shaders/volume_frag.glsl", GL_FRAGMENT_SHADER
		);
	}

	inline void initialize(const Camera& camera, GLint textureId, int screenWidth, int screenHeight) {
		data.renderCamera = std::make_unique<CameraReference>(camera);
		data.textureLibraryID = textureId;

		// TODO better asset system
		data.quadMesh = std::make_unique<Mesh>(GeometryLibrary::get(0), MaterialLibrary::get(0));
		data.sphereMesh = std::make_unique<Mesh>(GeometryLibrary::get(1), MaterialLibrary::get(0));
		//data.coneMesh = ResourceLoader::importModel("resources/cone.obj")[0];

		//sphereRadius = 0.45f; // sphere.obj
		data.sphereRadius = 5.93f; // sphere-hd.obj
		
		initFramebuffers(screenWidth, screenHeight);

		loadShaders();
	}

	template<size_t SCENE_ID>
	void buildLights(Scene<SCENE_ID>& scene) {
		// -- dirlights --

		auto dirLightView = scene.view<DirectionalLight, ExcludeComponent<Quad>>(); // TODO hack to make view work
		std::vector<DirectionalLight> dirLights;

		dirLightView.each([&](const DirectionalLight& d) {
			dirLights.push_back(d);
			data.dirLights.push_back(d.direction);
		});

		
		if (!dirLights.empty()) {
			std::vector<glm::mat4> dirLightMats(dirLights.size(), glm::mat4(1.0));

			UniformBufferTechnique::uploadDirectionalLightData(dirLights);
			data.quadMesh->updateInstances(dirLightMats);
		}
		
		// -- pointlights --

		auto pointLightView = scene.view<PointLight, ExcludeComponent<Quad>>(); // TODO hack to make view work
		std::vector<PointLight> pointLights;
		std::vector<glm::mat4> pointLightInstanceMats;

		pointLightView.each([&](const PointLight& p) {
			pointLights.push_back(p);
			pointLightInstanceMats.push_back(p.getTransformMatrix(data.sphereRadius));
			data.pointLights.push_back(p.position);
		});

		if (!pointLights.empty()) {
			UniformBufferTechnique::uploadPointLightData(pointLights);
			data.sphereMesh->updateInstances(pointLightInstanceMats);
		}

		int totalLights = data.dirLights.size() + data.pointLights.size();
		if (totalLights > data.shadowBuffer->specification.layers) {
			data.shadowBuffer->growDepthLayers(totalLights);
		}
	}


	inline void setupFrame() {
		OpenGL::depthTest(true);

		data.geometryShader->use();
		
		GLint texUnit = 0;
		glBindTextureUnit(texUnit, data.textureLibraryID);
		data.geometryShader->setUniform("textureLib", texUnit);
		
		data.gbuffer->clear();
		data.gbuffer->bindForWriting();

		data.geometryShader->setUniform("normal", glm::mat4(1.0));

		// render batches
		// TODO we don't do any batching right now - see render(scene) for process

		for (auto& batch : data.batches) {
			batch->bind();
			/*if (GLint texID = batch->textureID; data.currentlyBoundTexture != texID) {
				glBindTextureUnit(texUnit, texID);
				data.currentlyBoundTexture = texID;
			}*/
			glDrawElements(GL_TRIANGLES, batch->numIndices, GL_UNSIGNED_INT, nullptr);
			stats.drawCalls++;
		}
		Batch::unbind();
	}

	inline void render(const Model& model) {
		for (Mesh* mesh : *model.meshes) {
			mesh->bind();
			glDrawElementsInstanced(mesh->type(), mesh->numIndices(), GL_UNSIGNED_INT, nullptr, mesh->instanceCount);
			stats.drawCalls++;
		}
	}


	template<size_t SCENE_ID>
	void render(Scene<SCENE_ID>& scene) {
		auto bshBoundrary = [](glm::vec4 sphere) { return true; }; //Utility function combined with necessary rendering logic need to be applied here.
		
		if (buildBatch) {
			data.batches.clear();

			auto batchView = scene.view<PropertiesModel, Meshes, Properties3D, Position3D, Rotation3D, Opaque, ExcludeComponent<Instanced>>();
			batchView.each([](const PropertiesModel& propertiesModel, const Meshes& meshes,
							  const Properties3D& properties3D, const Position3D& position3D, const Rotation3D& rotation3D,
							  const Opaque&) {
				for (Mesh* mesh : meshes) {
					addToBatch(*mesh, Model::modelMatrix(position3D, rotation3D, properties3D));
				}
			});
			buildBatch = false;
		}
	}
	

	inline void shadowVolumePass() {
		glStencilFunc(GL_ALWAYS, 0, 0xFF);

		// set stencil test according to zfail algorithm
		glStencilOpSeparate(GL_BACK, GL_KEEP, GL_INCR_WRAP, GL_KEEP);
		glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_DECR_WRAP, GL_KEEP);
			
		// Clamp depth values at infinity to max depth. Required for back cap of volume to be included
		glEnable(GL_DEPTH_CLAMP);

		// do not render to depth or color buffer
		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
		glDepthMask(GL_FALSE);

		data.dirShadowShader->use();
		
		data.gbuffer->bindForReading();
		data.shadowBuffer->bindForWriting();

		glm::mat4 viewMat = data.renderCamera->viewMatrix();

		for (int i = 0; i < data.dirLights.size(); i++) {
			data.shadowBuffer->bindDepthLayer(i);
			Framebuffer::copyDepth(*data.gbuffer, *data.shadowBuffer);
			
			glm::vec4 lightDir = viewMat * glm::vec4(data.dirLights[i], 0.0f);
			data.dirShadowShader->setUniform("lightDir_viewSpace", glm::vec3(lightDir.x, lightDir.y, lightDir.z));
			
			for (Model* model : *data.shadowModels) {
				for (Mesh* mesh : *model->meshes) {
					KYSE_ASSERT(mesh->geometry->type == GL_TRIANGLES_ADJACENCY)
					mesh->bind();
					glDrawElementsInstanced(mesh->type(), mesh->numIndices(), GL_UNSIGNED_INT, nullptr, mesh->instanceCount);
					stats.drawCalls++;
				}
				Mesh::unbind();
			}
		}

		data.pointShadowShader->use();
		int numDirLights = data.dirLights.size();

		for (int i = 0; i < data.pointLights.size(); i++) {
			data.shadowBuffer->bindDepthLayer(i + numDirLights);
			Framebuffer::copyDepth(*data.gbuffer, *data.shadowBuffer);

			glm::vec4 lightViewPos = viewMat * glm::vec4(data.pointLights[i], 1.0f);
			data.pointShadowShader->setUniform("lightPos_viewSpace", glm::vec3(lightViewPos.x, lightViewPos.y, lightViewPos.z));
			
			for (Model* model : *data.shadowModels) {
				for (Mesh* mesh : *model->meshes) {
					KYSE_ASSERT(mesh->geometry->type == GL_TRIANGLES_ADJACENCY)
					mesh->bind();
					glDrawElementsInstanced(mesh->type(), mesh->numIndices(), GL_UNSIGNED_INT, nullptr, mesh->instanceCount);
					stats.drawCalls++;
				}
				Mesh::unbind();
			}
		}
		
		data.shadowBuffer->unbind();
		
		// reset to working state
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
		glDepthMask(GL_TRUE);
		
		glDisable(GL_DEPTH_CLAMP);

		if (drawShadowVolumes) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

			OpenGL::depthTest(false);

			if (!data.dirLights.empty()) {
				data.dirShadowShader->use();
				data.dirShadowShader->setUniform("lightDir", data.dirLights[0]);
				
				for (Model* model : *data.shadowModels) {
					for (Mesh* mesh : *model->meshes) {
						KYSE_ASSERT(mesh->geometry->type == GL_TRIANGLES_ADJACENCY);
						mesh->bind();
						glDrawElementsInstanced(mesh->type(), mesh->numIndices(), GL_UNSIGNED_INT, nullptr, mesh->instanceCount);
						stats.drawCalls++;
					}
					Mesh::unbind();
				}
			}

			if (!data.pointLights.empty()) {
				data.pointShadowShader->use();
				glm::vec4 lightViewPos = viewMat * glm::vec4(data.pointLights[0], 1.0f);
				data.pointShadowShader->setUniform("lightPos", glm::vec3(lightViewPos.x, lightViewPos.y, lightViewPos.z));
				
				for (Model* model : *data.shadowModels) {
					for (Mesh* mesh : *model->meshes) {
						KYSE_ASSERT(mesh->geometry->type == GL_TRIANGLES_ADJACENCY);
						mesh->bind();
						glDrawElementsInstanced(mesh->type(), mesh->numIndices(), GL_UNSIGNED_INT, nullptr, mesh->instanceCount);
						stats.drawCalls++;
					}
					Mesh::unbind();
				}
			}

			OpenGL::depthTest(true);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
	}

	inline void directionalLightPass() {
		data.dirShader->use();

		data.dirShader->setUniform("worldCameraPos", *data.renderCamera->position);
		
		data.gbuffer->bindColorTextureUnit(GBufferTextureType::Position);
		data.dirShader->setUniform("gPosition", GBufferTextureType::Position);
		data.gbuffer->bindColorTextureUnit(GBufferTextureType::Normal_Material);
		data.dirShader->setUniform("gNormal", GBufferTextureType::Normal_Material);
		data.gbuffer->bindColorTextureUnit(GBufferTextureType::Color_Specular);
		data.dirShader->setUniform("gColor", GBufferTextureType::Color_Specular);

		glBindTextureUnit(3, data.shadowBuffer->depthTexture);
		data.dirShader->setUniform("shadowTexture", 3);
		
		auto* mesh = data.quadMesh.get();
		mesh->bind();
		glDrawElementsInstanced(mesh->type(), mesh->numIndices(), GL_UNSIGNED_INT, nullptr, mesh->instanceCount);
		Mesh::unbind();
		
		stats.drawCalls++;
	}
	
	inline void pointLightPass() {
		data.pointShader->use();

		data.pointShader->setUniform("worldCameraPos", *data.renderCamera->position);

		data.gbuffer->bindColorTextureUnit(GBufferTextureType::Position);
		data.pointShader->setUniform("gPosition", GBufferTextureType::Position);
		data.gbuffer->bindColorTextureUnit(GBufferTextureType::Normal_Material);
		data.pointShader->setUniform("gNormal", GBufferTextureType::Normal_Material);
		data.gbuffer->bindColorTextureUnit(GBufferTextureType::Color_Specular);
		data.pointShader->setUniform("gColor", GBufferTextureType::Color_Specular);

		glBindTextureUnit(3, data.shadowBuffer->depthTexture);
		data.pointShader->setUniform("shadowTexture", 3);

		uint numDirLights = data.dirLights.size();
		data.pointShader->setUniform("dirLightCount", numDirLights);

		auto* mesh = data.sphereMesh.get();
		mesh->bind();
		glDrawElementsInstanced(mesh->type(), mesh->numIndices(), GL_UNSIGNED_INT, nullptr, mesh->instanceCount);
		Mesh::unbind();

		stats.drawCalls++;
	}
	

	inline void endFrame() {
		OpenGL::stencilTest(true);

		shadowVolumePass();
		Framebuffer::unbind();

		OpenGL::stencilTest(false);

		OpenGL::depthTest(false);

		OpenGL::blending(true);
		OpenGL::setBlendMode(GL_SRC_ALPHA, GL_ONE);
		
		data.gbuffer->bindForReading();
		directionalLightPass();

		OpenGL::cullFace(OpenGL::FRONT);
		
		pointLightPass();
		
		OpenGL::blending(false);
		OpenGL::cullFace(GL_NONE);
		data.gbuffer->unbind();
	}

	// Utility functions

	inline void rebuildBatch() {
		buildBatch = true;
	}

	inline void resizeFramebuffers(int width, int height) {
		data.gbuffer->resize(width, height);
		data.shadowBuffer->resize(width, height);
	}
	
	inline void resetStats() {
		memset(&stats, 0, sizeof(RenderStats));
	}

};
