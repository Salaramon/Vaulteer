#pragma once

#pragma once

#include <glm/glm.hpp>

#include "OpenGL.h"

#include "Renderer/DeferredRenderer.h"

#include "Renderer/Buffers/AlphaBuffer.h"

#include "API/Camera.h"
#include "Scene/Scene.h"


class ShadowVolumeRenderer {
	inline static std::unique_ptr<Framebuffer> frameBuffer;
	inline static Mesh* quad;

	inline static std::unique_ptr<Shader> shader;

public:
	static void initialize() {
		shader = std::make_unique<Shader>(
			"resources/shaders/volume_vertex.glsl", GL_VERTEX_SHADER,
			"resources/shaders/volume_geom.glsl", GL_GEOMETRY_SHADER,
			"resources/shaders/volume_frag.glsl", GL_FRAGMENT_SHADER
		);
	}
	
	static void reloadShaders() {
		
	}

	template<auto SCENE_ID>
	static void render(Scene<SCENE_ID>& scene) {
		auto cameraIteratorPair = scene.get<Camera>();
		auto cameraBeginIt = cameraIteratorPair.first;
		auto* camera = (*cameraBeginIt).get();

		blendingPass(scene, camera);
		
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		//drawShadowVolumes();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}


	template<size_t SCENE_ID>
	static void blendingPass(Scene<SCENE_ID>& scene, Camera* camera) {
		shader->use();

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

		//const auto modelDataIteratorPair = scene.get<OpaqueModel>(staticSceneRestriction);

		//ShadowVolumeTechnique::setInverseViewMatrix(camera->getViewMatrix());

		/*
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
		*/
	}
};

