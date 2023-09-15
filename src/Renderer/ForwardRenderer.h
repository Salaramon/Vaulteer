#pragma once

#include <glm/glm.hpp>

#include "API/Camera.h"

#include "Model/Resources/ResourcePack.h"
#include "Scene/Scene.h"
#include "Renderer/Shader.h"

namespace ForwardRenderer {

	inline struct ForwardRendererData {
		std::unique_ptr<Shader> shader;
		std::unique_ptr<CameraReference> renderCamera;
		GLint textureID;
	} data;
	
	inline struct RenderStats {
		size_t drawCalls = 0;
	} stats;

	inline void loadShaders() {
		data.shader = std::make_unique<Shader>(
			"resources/shaders/build/forward_vertex.glsl", GL_VERTEX_SHADER, 
			"resources/shaders/build/forward_frag.glsl", GL_FRAGMENT_SHADER
		);
	}

	inline void initialize(const Camera& camera, const GLint textureID) {
		data.renderCamera = std::make_unique<CameraReference>(camera);
		data.textureID = textureID;

		loadShaders();
	}

	inline void setupFrame() {
		OpenGL::stencilTest(true);

		data.shader->use();
		
		auto view = data.renderCamera->viewMatrix();
		data.shader->setUniform("inverseViewMat", glm::inverse(view));

		glBindTextureUnit(0, data.textureID);
		data.shader->setUniform("textureLib", 0);

		data.shader->setUniform("cameraPos", *data.renderCamera->position);
		data.shader->setUniform("lightPos", glm::vec3(5.0));
	}

	inline void render(const Model& model) {
		auto modelMat = model.getModelMatrix();
		glm::mat4 normalMat = glm::mat4(1.0);
		if (model.hasScaleFactor()) {
			normalMat = (model.hasUniformScale() ? modelMat : glm::transpose(glm::inverse(modelMat)));
		}
		data.shader->setUniform("normal", normalMat);

		for (auto mesh : *model.meshes) {
			mesh->bind();
			glDrawElementsInstanced(mesh->type(), mesh->numIndices(), GL_UNSIGNED_INT, nullptr, mesh->instanceCount);
			stats.drawCalls++;
		}
		Mesh::unbind();
	}

	inline void endFrame() {
		OpenGL::stencilTest(false);
	}
}
