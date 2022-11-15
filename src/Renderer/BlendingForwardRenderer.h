#pragma once

#include <glm/glm.hpp>

#include "OpenGL.h"

#include "Renderer/DeferredRenderer.h"

#include "Renderer/RendererPrerequisites.h"
#include "Renderer/Buffers/AlphaBuffer.h"
#include "Renderer/Techniques/BlendingTechnique.h"
#include "Renderer/Techniques/BlendingCompositeTechnique.h"

#include "Scene/StaticScene.h"
#include "Scene/DynamicScene.h"

#include "Model/Data/ModelData.h"
#include "Model/Data/LineData.h"

#include "API/Camera.h"

#include "Renderer/Tags/Transparent.h"

class BlendingForwardRenderer : public RendererPrerequisites<DynamicScene<Camera>, StaticScene<TransparentModel>>, 
	public BlendingTechnique, public BlendingCompositeTechnique {
	using AlphaTexType = AlphaBuffer::AlphaBufferTextureType;

	inline static std::unique_ptr<AlphaBuffer> alphaBuffer;
	inline static std::unique_ptr<ModelData> quad;

public:
	static void initialize(uint screenWidth, uint screenHeight);

	static void rebuildAlphaBuffer(int width, int height);
	
	static void reloadShaders();

	template<class... Args1, class... Args2>
	static void render(DynamicScene<Args1...>& dynamicScene, StaticScene<Args2...>& staticScene) {
		auto cameraIteratorPair = dynamicScene.get<Camera>();
		auto cameraBeginIt = cameraIteratorPair.first;
		auto* camera = (*cameraBeginIt).get();

		OpenGL::enableBlending();
		blendingPass(staticScene, camera);

		DeferredRenderer::copyGBufferDepth(alphaBuffer->fbo);

		compositePass(camera);
		OpenGL::disableBlending();
	}

	template<class... Args2>
	static void blendingPass(StaticScene<Args2...>& staticScene, Camera* camera) {
		BlendingTechnique::shader->use();

		glDepthMask(GL_FALSE);
		OpenGL::enableDepthTest();
		OpenGL::setBlendMode(AlphaTexType::Accumulated, OpenGL::BlendModes::One, OpenGL::BlendModes::One);
		OpenGL::setBlendMode(AlphaTexType::Alpha, OpenGL::BlendModes::Zero, OpenGL::BlendModes::OneMinusSourceColor);
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

		const auto modelDataIteratorPair = staticScene.get<TransparentModel>(staticSceneRestriction);

		BlendingTechnique::setInverseViewMatrix(camera->getViewMatrix());

		for (auto it = modelDataIteratorPair.first; it != modelDataIteratorPair.second; it++) {
			auto& model = (*it).get()->model;
			ModelData* modelData = model.getData();
			BlendingTechnique::setModelView(model.getModelMatrix(), camera->getViewMatrix());

			std::vector<Mesh>& modelDataMeshes = modelData->getMeshes();

			for (Mesh& mesh : modelDataMeshes) {
				mesh.bind();
				glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, nullptr);
				mesh.unbind();
			}
		}
		alphaBuffer->unbind();

		glDepthMask(GL_TRUE);
	}

	static void compositePass(Camera* camera) {
		BlendingCompositeTechnique::shader->use();

		OpenGL::setBlendMode(OpenGL::BlendModes::SourceAlpha, OpenGL::BlendModes::OneMinusSourceAlpha);

		//glDepthFunc(GL_LESS); // less is the default

		DeferredRenderer::copyGBufferDepth(0); // copy to backbuffer FBO

		alphaBuffer->bindForReading();

		alphaBuffer->bindTextureUnit(AlphaTexType::Accumulated);
		alphaBuffer->bindTextureUnit(AlphaTexType::Alpha);
		BlendingCompositeTechnique::setTextureUnits(AlphaTexType::Accumulated, AlphaTexType::Alpha);

		Mesh& quadMesh = quad->getMeshes().front();
		quadMesh.bind();
		glDrawElements(GL_TRIANGLES, quadMesh.indices.size(), GL_UNSIGNED_INT, nullptr);
		quadMesh.unbind();

		alphaBuffer->unbind();
	}
};
