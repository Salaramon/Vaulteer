#pragma once

#include <type_traits>

#include <glm/glm.hpp>

#include "OpenGL.h"

#include "Renderer/RendererPrerequisites.h"
#include "Renderer/Techniques/ForwardTechnique.h"
#include "Renderer/Techniques/LineTechnique.h"
#include "Scene/StaticScene.h"
#include "Scene/DynamicScene.h"

#include "Model/Model.h"
#include "Model/Data/ModelData.h"
#include "Model/Data/LineData.h"
#include "API/Camera.h"

#include "Transparent.h"

class BlendingForwardRenderer : public RendererPrerequisites<DynamicScene<Camera>, StaticScene<TransparentModel>>, public ForwardTechnique {
public:

	template<class... Args1, class... Args2>
	static void render(DynamicScene<Args1...>& dynamicScene, StaticScene<Args2...>& staticScene) {
		ForwardTechnique::shader->use();

		OpenGL::enableDepthTest();
		OpenGL::enableBlending();
		OpenGL::setBlendMode(OpenGL::BlendModes::SourceAlpha, OpenGL::BlendModes::OneMinusSourceAlpha);

		auto cameraIteratorPair = dynamicScene.get<Camera>();
		auto cameraBeginIt = cameraIteratorPair.first;
		auto* camera = (*cameraBeginIt).get();

		//NEEDS TO BE CHANGED TO FRUSTUM SHAPE INSTEAD OF SPHERE SHAPE
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


		ForwardTechnique::setInverseViewMatrix(camera->getViewMatrix());

		for (auto it = modelDataIteratorPair.first; it != modelDataIteratorPair.second; it++) {
			auto& model = (*it).get()->model;
			ModelData* modelData = model.getData();
			ForwardTechnique::setModelView(model.getModelMatrix(), camera->getViewMatrix());

			std::vector<Mesh>& modelDataMeshes = modelData->getMeshes();

			for (Mesh& mesh : modelDataMeshes) {
				mesh.bind();
				glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, 0);
				mesh.unbind();
			}
		}

		OpenGL::disableBlending();
	}

};
