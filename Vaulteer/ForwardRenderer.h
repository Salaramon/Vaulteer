#pragma once

#include <type_traits>

#include <glm/glm.hpp>

#include "RendererPrerequisites.h"
#include "ForwardTechnique.h"
#include "LineTechnique.h"
#include "StaticScene.h"
#include "DynamicScene.h"

#include "Model.h"
#include "ModelData.h"
#include "LineData.h"
#include "Camera.h"



class ForwardRenderer : public RendererPrerequisites<DynamicScene<Camera>, StaticScene<Model<ModelData>, Model<LineData>>>, public ForwardTechnique, public LineTechnique {
public:

	template<class... Args1, class... Args2>
	static void render(DynamicScene<Args1...>& dynamicScene, StaticScene<Args2...>& staticScene) {
		ForwardTechnique::shader->use();

		/*auto cameraIteratorPair = dynamicScene.get<Camera>();
		auto cameraBeginIt = cameraIteratorPair.first;
		auto& camera = *cameraBeginIt;


		//NEEDS TO BE CHANGED TO FRUSTUM SHAPE INSTEAD OF SPHERE SHAPE
		auto staticSceneRestriction = [&](glm::vec4 sphere) -> bool {

			Camera::Frustum frustum = camera->getFrustum();

			glm::vec3 cameraPosition = camera->getPosition();

			float cameraInfluenceRadius = 50;
			glm::vec3 spherePoint = glm::vec3(sphere);

			float dist = glm::distance(cameraPosition, spherePoint);
			float radSum = cameraInfluenceRadius + sphere.w;
			bool result = dist < radSum;
			return result;
		};

		const auto modelDataIteratorPair = staticScene.get<Model<ModelData>>(staticSceneRestriction);

		ForwardTechnique::setProjection(camera->getProjectionMatrix());
		ForwardTechnique::setView(camera->getViewMatrix());

		for (auto it = modelDataIteratorPair.first; it != modelDataIteratorPair.second; it++) {
			auto& itValue = (*it);
			ForwardTechnique::setModel(itValue->getModelMatrix());

			ModelData* modelData = itValue->getData();
			const std::vector<Mesh>& modelDataMeshes = modelData->getMeshes();
			const std::vector<Texture>& modelDataTextures = modelData->getTextures();

			for (const Mesh& mesh : modelDataMeshes) {
				for (GLint i = 0; i < modelDataTextures.size(); i++) {
					modelDataTextures[i].activate(*ForwardTechnique::shader, i);
				}

				mesh.vertexArray.bind();
				//glDrawElementsInstanced(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, 0, instances.size());
				//debug("Drawing mesh: " + std::to_string(mesh.getObjectKey()) + "\n", "glDrawElements");
				glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, 0);
				mesh.vertexArray.unbind();

				//Is setting active texture back to 0 unnecessary?
				glActiveTexture(GL_TEXTURE0);
			}
		}




		LineTechnique::shader->use();

		auto lineDataIteratorPair = staticScene.get<Model<LineData>>(staticSceneRestriction);

		LineTechnique::setProjection(camera->getProjectionMatrix());
		LineTechnique::setView(camera->getViewMatrix());
		for (auto it = lineDataIteratorPair.first; it != lineDataIteratorPair.second; it++) {
			auto& itValue = (*it);

			LineTechnique::setModel(itValue->getModelMatrix());

			LineData* lineData = itValue->getData();
			LineTechnique::setColor(lineData->color);

			lineData->vertexArray.bind();
			glDrawElements(GL_LINES, lineData->indices.size(), GL_UNSIGNED_INT, 0);
			lineData->vertexArray.unbind();
			//Is setting active texture back to 0 unnecessary?
			glActiveTexture(GL_TEXTURE0);
		}
		*/
	}
};