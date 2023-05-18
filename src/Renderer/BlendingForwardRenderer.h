#pragma once

#include <glm/glm.hpp>

#include "OpenGL.h"

#include "Renderer/DeferredRenderer.h"
#include "Renderer/Buffers/AlphaBuffer.h"

#include "API/Camera.h"
#include "SceneTypedefs.h"
#include "Scene/Scene.h"


class BlendingForwardRenderer  {
	using AlphaTexType = AlphaBuffer::AlphaBufferTextureType;

	inline static std::unique_ptr<AlphaBuffer> alphaBuffer;
	inline static Mesh* quadMesh;
	
	inline static std::unique_ptr<Shader> blendingShader;
	inline static std::unique_ptr<Shader> compositeShader;
		
public:
	static void initialize(uint screenWidth, uint screenHeight) {
		alphaBuffer = std::make_unique<AlphaBuffer>(screenWidth, screenHeight);

		quadMesh = ResourceLoader::importModel("resources/quad.obj")[0];

		loadShaders();
	}

	static void loadShaders() {
		blendingShader = std::make_unique<Shader>(
			"resources/shaders/blending_vertex.glsl", GL_VERTEX_SHADER,
			"resources/shaders/blending_frag.glsl", GL_FRAGMENT_SHADER
		);
		compositeShader = std::make_unique<Shader>(
			"resources/shaders/blending_composite_vertex.glsl", GL_VERTEX_SHADER,
			"resources/shaders/blending_composite_frag.glsl", GL_FRAGMENT_SHADER
		);
	}

	static void rebuildAlphaBuffer(int width, int height) {
		alphaBuffer.reset();
		alphaBuffer = std::make_unique<AlphaBuffer>(width, height);
	}

	template<size_t SCENE_ID>
	static void render(Scene<SCENE_ID>& scene) {
		{
			glDepthMask(GL_FALSE);
			OpenGL::enableDepthTest();
			OpenGL::enableBlending();

			blendingPass(scene);
			
			glDepthMask(GL_TRUE);
		}

		{
			// TODO: put this in gbuffer or something
			DeferredRenderer::copyGBufferDepth(alphaBuffer->fbo);

			compositePass();

			OpenGL::disableBlending();
		}
	}

	template<size_t SCENE_ID>
	static void blendingPass(Scene<SCENE_ID>& scene) {
		blendingShader->use();
		
		auto camera = scene.getActiveCamera();

		OpenGL::setBlendMode(AlphaTexType::Accumulated, GLBlendModes::One, GLBlendModes::One);
		OpenGL::setBlendMode(AlphaTexType::Alpha, GLBlendModes::Zero, GLBlendModes::OneMinusSourceColor);
		glBlendEquation(GL_FUNC_ADD);
		
		// TODO: NEEDS TO BE CHANGED TO FRUSTUM SHAPE
		auto staticSceneRestriction = [&](glm::vec4 sphere) -> bool {
			return true;
			/*Camera::Frustum frustum = camera->getFrustum();

			glm::vec3 cameraPosition = camera->getPosition();

			float cameraInfluenceRadius = 50;
			glm::vec3 spherePoint = glm::vec3(sphere);

			float dist = glm::distance(cameraPosition, spherePoint);
			float radSum = cameraInfluenceRadius + sphere.w;
			bool result = dist < radSum;
			return result;*/
		};

		auto modelView = scene.view<PropertiesModel, Properties3D, Meshes, Position3D, Rotation3D>();

		blendingShader->setUniform("inverseViewMat", glm::inverse(camera.viewMatrix())); // glsl inverse is not the same as glm inverse...
		blendingShader->setUniform("view", camera.viewMatrix());

		blendingShader->setUniform("textureLib", 0);
		
		alphaBuffer->clear();
		alphaBuffer->bindForWriting();
		
		modelView.each([](const PropertiesModel&, const Properties3D& properties3D, const Meshes& meshes, const Position3D& position3D, const Rotation3D& rotation3D) {
			blendingShader->setUniform("model", Model::modelMatrix(position3D, rotation3D, properties3D));

			for (auto mesh : meshes) {
				mesh->bind();
				glDrawElements(GL_TRIANGLES, mesh->indices.size(), GL_UNSIGNED_INT, nullptr);
				mesh->unbind();
			}
		});

		alphaBuffer->unbind();
	}

	static void compositePass() {
		compositeShader->use();

		OpenGL::setBlendMode(GLBlendModes::SourceAlpha, GLBlendModes::OneMinusSourceAlpha);

		//glDepthFunc(GL_LESS); // less is the default

		DeferredRenderer::copyGBufferDepth(0); // copy to backbuffer FBO

		alphaBuffer->bindForReading();

		compositeShader->setUniform("accum", AlphaTexType::Accumulated);
		compositeShader->setUniform("reveal", AlphaTexType::Alpha);

		quadMesh->bind();
		glDrawElements(GL_TRIANGLES, quadMesh->indices.size(), GL_UNSIGNED_INT, nullptr);
		quadMesh->unbind();

		alphaBuffer->unbind();
	}
};
