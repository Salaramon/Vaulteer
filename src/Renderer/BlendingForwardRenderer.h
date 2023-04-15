#pragma once

#include <glm/glm.hpp>

#include "OpenGL.h"

#include "Renderer/DeferredRenderer.h"

#include "Renderer/Buffers/AlphaBuffer.h"
#include "Renderer/Techniques/BlendingTechnique.h"
#include "Renderer/Techniques/BlendingCompositeTechnique.h"

#include "Scene/StaticScene.h"
#include "Scene/DynamicScene.h"

#include "Model/Data/ModelData.h"

#include "API/Camera.h"

#include "SceneTypedefs.h"

class BlendingForwardRenderer :
	public BlendingTechnique, 
	public BlendingCompositeTechnique {
	using AlphaTexType = AlphaBuffer::AlphaBufferTextureType;

	inline static std::unique_ptr<AlphaBuffer> alphaBuffer;
	inline static Mesh* quadMesh;

public:
	static void initialize(uint screenWidth, uint screenHeight);

	static void rebuildAlphaBuffer(int width, int height);
	
	static void reloadShaders();

	template<size_t SCENE_ID>
	static void render(Scene<SCENE_ID>& scene) {
		/*

		auto camera = scene.view<A,ctiveCamera>();

		OpenGL::enableBlending();
		blendingPass(scene, camera);

		DeferredRenderer::copyGBufferDepth(alphaBuffer->fbo);

		compositePass(camera);
		OpenGL::disableBlending();
		*/
	}

	template<size_t SCENE_ID>
	static void blendingPass(Scene<SCENE_ID>& scene) {
		//BlendingTechnique::use();

		glDepthMask(GL_FALSE);
		OpenGL::enableDepthTest();
		OpenGL::setBlendMode(AlphaTexType::Accumulated, GLBlendModes::One, GLBlendModes::One);
		OpenGL::setBlendMode(AlphaTexType::Alpha, GLBlendModes::Zero, GLBlendModes::OneMinusSourceColor);
		glBlendEquation(GL_FUNC_ADD);

		alphaBuffer->clear();
		alphaBuffer->bindForWriting();

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



		// TODO: following needs to be rewritten to new scene handling


		//const auto modelDataIteratorPair = scene.get<TransparentModel>(staticSceneRestriction);

		//BlendingTechnique::setInverseViewMatrix(camera->getViewMatrix());
		//BlendingTechnique::setView(camera->getViewMatrix());

		/*
		for (auto it = modelDataIteratorPair.first; it != modelDataIteratorPair.second; it++) {
			auto& model = (*it).get()->model;
			ModelData* modelData = model.getData();
			BlendingTechnique::setModel(model.getModelMatrix());

			std::vector<Mesh>& modelDataMeshes = modelData->getMeshes();

			for (Mesh& mesh : modelDataMeshes) {
				mesh.bind();
				glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, nullptr);
				mesh.unbind();
			}
		}
		alphaBuffer->unbind();

		glDepthMask(GL_TRUE);
		*/
	}

	static void compositePass(Camera* camera) {
		//BlendingCompositeTechnique::use();

		OpenGL::setBlendMode(GLBlendModes::SourceAlpha, GLBlendModes::OneMinusSourceAlpha);

		//glDepthFunc(GL_LESS); // less is the default

		DeferredRenderer::copyGBufferDepth(0); // copy to backbuffer FBO

		alphaBuffer->bindForReading();

		alphaBuffer->bindTextureUnit(AlphaTexType::Accumulated);
		alphaBuffer->bindTextureUnit(AlphaTexType::Alpha);
		BlendingCompositeTechnique::setTextureUnits(AlphaTexType::Accumulated, AlphaTexType::Alpha);

		quadMesh->bind();
		glDrawElements(GL_TRIANGLES, quadMesh->indices.size(), GL_UNSIGNED_INT, nullptr);
		quadMesh->unbind();

		alphaBuffer->unbind();
	}
};
