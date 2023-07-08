#pragma once

#pragma once

#include <glm/glm.hpp>

#include "OpenGL.h"

#include "Renderer/DeferredRenderer.h"

#include "API/Camera.h"
#include "Scene/Scene.h"


class ShadowVolumeRenderer {
	inline static DirectionalLight light;

	inline static std::unique_ptr<Framebuffer> frameBuffer;
	inline static Mesh* quad;
	
	inline static std::unique_ptr<Shader> prepassShader;
	inline static std::unique_ptr<Shader> volumeShader;

public:	
	inline static std::unique_ptr<Framebuffer> shadowbuffer;

	inline static bool drawVolumes = false;

	static void initialize(const DirectionalLight light) {
		ShadowVolumeRenderer::light = light;

		FramebufferSpecification shadowbufferSpec {
			1280, 720,
			{{.textureFormat = GL_DEPTH24_STENCIL8, .depthStencilMode = GL_STENCIL_INDEX}},
			GL_TEXTURE_2D_ARRAY, initialShadowMapCount
		};
		shadowbuffer = std::make_unique<Framebuffer>(shadowbufferSpec);

		loadShaders();
	}
	 
	static void loadShaders() {		
		prepassShader = std::make_unique<Shader>(
			"resources/shaders/volume_prepass_vertex.glsl", GL_VERTEX_SHADER
		);	
		volumeShader = std::make_unique<Shader>(
			"resources/shaders/volume_vertex.glsl", GL_VERTEX_SHADER,
			"resources/shaders/volume_geom.glsl", GL_GEOMETRY_SHADER,
			"resources/shaders/volume_frag.glsl", GL_FRAGMENT_SHADER
		);
	}

	template<auto SCENE_ID>
	static void render(Scene<SCENE_ID>& scene) {

		// -- depth pass (for testing, we should use depth from geometry pass later)
		
		shadowbuffer->bindForWriting();
		shadowbuffer->clearDepthStencil();

		OpenGL::stencilTest(false);

		// do not render to color buffer 
		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

		prepassShader->use();

		auto modelView = scene.view<PropertiesModel, Meshes, Position3D, Rotation3D, Properties3D>();
		modelView.each([](const PropertiesModel&, const Meshes& meshes, 
						  const Position3D& position, const Rotation3D& rotation, const Properties3D& properties3D) {
			auto modelMat = Object3D::modelMatrix(position, rotation, properties3D);
			prepassShader->setUniform("model", modelMat);

			for (auto mesh : meshes) {
				mesh->bind();
				glDrawElements(mesh->getType(), mesh->getNumIndices(), GL_UNSIGNED_INT, nullptr);
			}
			Mesh::unbind();
		});

		// -- drawShadowVolumes

		auto camera = scene.getActiveCamera();
		auto view = camera.viewMatrix();
		
		glm::vec3 lightPos = light.direction * -100.f;
		glm::vec4 lightViewPos = view * glm::vec4(lightPos, 1.0f);

		volumeShader->use();
		volumeShader->setUniform("lightPos", glm::vec3(lightViewPos.x, lightViewPos.y, lightViewPos.z));

		OpenGL::stencilTest(true);
		OpenGL::depthTest(true);


		// need stencil test to be enabled but we want it to succeed always. 
		// Only the depth test matters.
		glStencilFunc(GL_ALWAYS, 0, 0xFF);  // Set all stencil values to 0

		// set stencil test according to zfail algorithm
		glStencilOpSeparate(GL_BACK, GL_KEEP, GL_INCR_WRAP, GL_KEEP);
		glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_DECR_WRAP, GL_KEEP);
			
		// Clamp depth values at infinity to max depth. Required for back cap of volume to be included
		// (Obs! requires depth test GL_EQUAL to include max value. GL_LESS is not enough)
		glEnable(GL_DEPTH_CLAMP);

		// do not render to depth or color buffer 
		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
		glDepthMask(GL_FALSE);


		drawShadowVolumes(scene);
		shadowbuffer->unbind();


		/*
		// only draw if corresponding value in stencil buffer is zero
		glStencilFunc(GL_EQUAL, 0x0, 0xFF);

		// prevent update to the stencil buffer
		glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
		*/

		// reset to working state
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
		glDepthMask(GL_TRUE);
		
		glDepthFunc(GL_LESS);
		glDisable(GL_DEPTH_CLAMP);
		glClear(GL_DEPTH_BUFFER_BIT);
		
		if (drawVolumes) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

			drawShadowVolumes(scene);
			
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
	}
	
	
	template<auto SCENE_ID>
	static void drawShadowVolumes(Scene<SCENE_ID>& scene) {
		auto shadowModelView = scene.view<PropertiesModel, Meshes, Position3D, Rotation3D, Properties3D, Shadow>();

		shadowModelView.each([](const PropertiesModel&, const Meshes& meshes, 
						  const Position3D& position, const Rotation3D& rotation, const Properties3D& properties3D, 
						  const Shadow&) {
			auto modelMat = Object3D::modelMatrix(position, rotation, properties3D);
			volumeShader->setUniform("model", modelMat);

			for (auto mesh : meshes) {
				mesh->bind();
				glDrawElements(mesh->getType(), mesh->getNumIndices(), GL_UNSIGNED_INT, nullptr);
			}
			Mesh::unbind();
		});
	}
};

