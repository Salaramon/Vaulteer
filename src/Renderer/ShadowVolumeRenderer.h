#pragma once

#pragma once

#include <glm/glm.hpp>

#include "OpenGL.h"

#include "Renderer/DeferredRenderer.h"

#include "Renderer/RendererPrerequisites.h"
#include "Renderer/Buffers/AlphaBuffer.h"
#include "Renderer/Techniques/ShadowVolumeTechnique.h"

#include "Scene/StaticScene.h"
#include "Scene/DynamicScene.h"

#include "Model/Data/ModelData.h"

#include "API/Camera.h"

#include "Renderer/Tags/Opaque.h"

class ShadowVolumeRenderer : public RendererPrerequisites<DynamicScene<Camera>, StaticScene<OpaqueModel>>, 
	public ShadowVolumeTechnique {
	inline static std::unique_ptr<FrameBuffer> frameBuffer;
	inline static std::unique_ptr<ModelData> quad;

public:
	static void initialize(uint screenWidth, uint screenHeight);
	
	static void reloadShaders();

	template<class... Args1, class... Args2>
	static void render(DynamicScene<Args1...>& dynamicScene, StaticScene<Args2...>& staticScene) {
		auto cameraIteratorPair = dynamicScene.get<Camera>();
		auto cameraBeginIt = cameraIteratorPair.first;
		auto* camera = (*cameraBeginIt).get();

		blendingPass(staticScene, camera);

		
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		//drawShadowVolumes();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	template<class... Args2>
	static void blendingPass(StaticScene<Args2...>& staticScene, Camera* camera) {
		ShadowVolumeTechnique::use();

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

		const auto modelDataIteratorPair = staticScene.get<OpaqueModel>(staticSceneRestriction);

		ShadowVolumeTechnique::setInverseViewMatrix(camera->getViewMatrix());

		for (auto it = modelDataIteratorPair.first; it != modelDataIteratorPair.second; it++) {
			auto& model = (*it).get()->model;
			ModelData* modelData = model.getData();
			ShadowVolumeTechnique::setModelView(model.getModelMatrix(), camera->getViewMatrix());

			std::vector<Mesh>& modelDataMeshes = modelData->getMeshes();

			for (Mesh& mesh : modelDataMeshes) {
				mesh.bind();
				glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, nullptr);
				mesh.unbind();
			}
		}
	}
};

