#pragma once

#include <glm/glm.hpp>

#include "OpenGL.h"

#include "API/Camera.h"

#include "SceneTypedefs.h"
#include "Techniques/UniformBufferTechnique.h"

class ForwardRenderer {

	inline static GLint textureID;

	inline static std::unique_ptr<Shader> shader;

public:
	static void initialize(const ResourcePack& pack) {
		textureID = pack.getTextureID();
		loadShaders();
	}

	static void loadShaders() {
		gem::Shader<gem::forward_frag> ff;
		ff.setforward_frag_materialData(128);
		ff.compile();

		shader = std::make_unique<Shader>(
			"resources/shaders/forward_vertex.glsl", GL_VERTEX_SHADER, 
			"resources/shaders/build/forward_frag.glsl", GL_FRAGMENT_SHADER
		);
	}

	template<size_t SCENE_ID>
	static void render(Scene<SCENE_ID>& scene) {
		shader->use();
		
		auto camera = scene.getActiveCamera();
		auto modelView = scene.view<PropertiesModel, Meshes, Position3D, Rotation3D, Properties3D>();

		auto viewMat = camera.viewMatrix();
		auto projectionMat = camera.projectionMatrix();
		shader->setUniform("view", viewMat);

		glBindTextureUnit(0, textureID);
		shader->setUniform("textureLib", 0);

		shader->setUniform("inverseViewMat", glm::inverse(viewMat));

		shader->setUniform("cameraPos", *camera.position);
		shader->setUniform("lightPos", glm::vec3(0.0));

		
		modelView.each([](const PropertiesModel&, const Meshes& meshes, const Position3D& position, const Rotation3D& rotation, const Properties3D& properties3D) {
			auto modelMat = Object3D::modelMatrix(position, rotation, properties3D);

			shader->setUniform("model", modelMat);
			shader->setUniform("normal", glm::inverse(modelMat));

			for (auto mesh : meshes) {
				mesh->bind();
				glDrawElements(GL_TRIANGLES, mesh->indices.size(), GL_UNSIGNED_INT, nullptr);
				mesh->unbind();
			}
		});
	}

};
