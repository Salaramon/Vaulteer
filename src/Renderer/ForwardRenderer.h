#pragma once

#include <glm/glm.hpp>

#include "API/Camera.h"

#include "Scene/Scene.h"
#include "Model/Resources/ResourcePack.h"

class ForwardRenderer {

	inline static GLint textureID;

	inline static std::unique_ptr<Shader> shader;

public:
	static void initialize(const GLint textureId) {
		ForwardRenderer::textureID = textureId;
		loadShaders();
	}

	static void loadShaders() {
		gem::Shader<gem::forward_vertex> fv;
		fv.setforward_vertex_materialData(max_material_count);
		fv.compile();
		gem::Shader<gem::forward_frag> ff;
		ff.setforward_frag_materialData(max_material_count);
		ff.compile();

		shader = std::make_unique<Shader>(
			"resources/shaders/build/forward_vertex.glsl", GL_VERTEX_SHADER, 
			"resources/shaders/build/forward_frag.glsl", GL_FRAGMENT_SHADER
		);
	}

	template<size_t SCENE_ID>
	static void render(Scene<SCENE_ID>& scene) {
		shader->use();
		
		auto camera = scene.getActiveCamera();
		auto modelView = scene.view<PropertiesModel, Meshes, Position3D, Rotation3D, Properties3D>();

		auto viewMat = camera.viewMatrix();
		shader->setUniform("view", viewMat);
		shader->setUniform("inverseViewMat", glm::inverse(viewMat));

		glBindTextureUnit(0, textureID);
		shader->setUniform("textureLib", 0);

		shader->setUniform("cameraPos", *camera.position);
		shader->setUniform("lightPos", glm::vec3(5.0));

		
		modelView.each([](const PropertiesModel&, const Meshes& meshes, const Position3D& position, const Rotation3D& rotation, const Properties3D& properties3D) {
			auto modelMat = Object3D::modelMatrix(position, rotation, properties3D);

			shader->setUniform("model", modelMat);

			glm::mat4 normalMat;
			if (properties3D.scale == glm::vec3(1.0))
				normalMat = modelMat;
			else
				normalMat = glm::transpose(glm::inverse(modelMat));

			shader->setUniform("normal", normalMat);

			for (auto mesh : meshes) {
				mesh->bind();
				glDrawElements(GL_TRIANGLES, mesh->indices.size(), GL_UNSIGNED_INT, nullptr);
				mesh->unbind();
			}
		});
	}

};
