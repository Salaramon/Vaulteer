#pragma once

#include "Model/VertexArray.h"
#include "Model/Storage/QuadVertex.h"
#include "Techniques/UniformBufferTechnique.h"

#include "Text/TextLoader.h"
#include "Renderer/Shader.h"

class TextRenderer {
public:
	inline static std::unique_ptr<VertexArray> vertexArray;
	inline static VertexBuffer* vertexBuffer;
	inline static IndexBuffer* indexBuffer;

	inline static glm::mat4 projection;

	inline static std::unique_ptr<Shader> shader;

	static void initialize(uint screenWidth, uint screenHeight) {
		buildScreenProjection(screenWidth, screenHeight);

		vertexArray = std::make_unique<VertexArray>();
		vertexBuffer = vertexArray->createVertexBuffers(QuadVertex::getFormat()).at(0);
		indexBuffer = vertexArray->createIndexBuffer();
		
		auto a = Quad(40,40,40,40);
		vertexBuffer->insert(a.vertices.data(), 4);
		indexBuffer->insert(a.indices.data(), 6);

		TextLoader l;
		l.loadTexture();

		loadShaders();
	}

	static void loadShaders() {
		shader = std::make_unique<Shader>(
			"resources/shaders/text_vertex.glsl", GL_VERTEX_SHADER, 
			"resources/shaders/text_frag.glsl", GL_FRAGMENT_SHADER
		);
	}


	template<size_t SCENE_ID>
	static void render(Scene<SCENE_ID>& scene) {
		shader->use();

		glBindTextureUnit(0, 1);
		shader->setUniform("font", 0);

		vertexArray->bind();
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
		vertexArray->unbind();
	}


	static void buildScreenProjection(uint screenWidth, uint screenHeight) {
		projection = getScreenProjectionMatrix(screenWidth, screenHeight);
		UniformBufferTechnique::uploadScreenProjection(projection);
	}

	// orthographic projection matrix with size of screen
	static glm::mat4 getScreenProjectionMatrix(uint w, uint h) {
		return {
			 2.f/w, 0,  0, 0,
			 0,-2.f/h,  0, 0,
			 0,     0, -1, 0,
			-1,     1,  0, 1
		};
	}
};
