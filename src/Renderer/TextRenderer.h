#pragma once

#include "Model/VertexArray.h"
#include "Model/Storage/QuadVertex.h"
#include "Techniques/UniformBufferTechnique.h"

#include "Text/FontLibrary.h"
#include "Renderer/Shader.h"

class TextRenderer {
public:
	inline static std::unique_ptr<VertexArray> vertexArray;
	inline static VertexBuffer* vertexBuffer;
	inline static IndexBuffer* indexBuffer;

	inline static std::vector<QuadVertex> vertices;
	inline static std::vector<GLuint> indices;

	inline static glm::mat4 projection;

	inline static std::unique_ptr<Shader> shader;
	inline static uint numQuads;

	inline static Font* font;
	inline static glm::vec2 screenMiddle;

	static void initialize(uint screenWidth, uint screenHeight) {
		buildScreenProjection(screenWidth, screenHeight);

		vertexArray = std::make_unique<VertexArray>();
		vertexBuffer = vertexArray->createVertexBuffers(QuadVertex::getFormat()).at(0);
		indexBuffer = vertexArray->createIndexBuffer();

		font = FontLibrary::loadFont({"Georgia", "resources/fonts/georgia/msdf.json", "resources/fonts/georgia/msdf.png"});

		screenMiddle = {screenWidth/2,screenHeight/2};

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

		OpenGL::disableDepthTest();
		OpenGL::enableBlending();
		OpenGL::setBlendMode(GLBlendModes::SourceAlpha, GLBlendModes::One);

		glBindTextureUnit(0, font->mapTexture->textureID);
		shader->setUniform("msdf", 0);

		buildSomeText();

		vertexArray->bind();
		glDrawElements(GL_TRIANGLES, numQuads * 6, GL_UNSIGNED_INT, nullptr);
		vertexArray->unbind();

		OpenGL::disableBlending();
		OpenGL::enableDepthTest();
	}



	static void buildSomeText() {
		std::string content;
		for (int i = 0; i < 1000; i++) {
			content += rand() % 128 + 32;
			if ((i - 40) % 100 == 0) content += '\n';
		}

		vertices.clear();
		indices.clear();

		auto [lineSize, quads] = FontLibrary::stringToQuads(font, content, screenMiddle, 1.0);
		numQuads = quads.size();
		
		int i = 0;
		for (auto& quad : quads) {
			quad.x -= lineSize.x / 2;
			quad.y -= lineSize.y / 2;

			auto v = quad.vertices();
			vertices.insert(vertices.end(), std::begin(v), std::end(v));

			std::array<uint, 6> quadInd = quad.indices();
			for (int j = 0; j < 6; j++) {
				quadInd[j] += 4 * i;
			}

			indices.insert(indices.end(), std::begin(quadInd), std::end(quadInd));
			i++;
		}

  		vertexBuffer->insert(vertices.data(), vertices.size());
		indexBuffer->insert(indices.data(), indices.size());
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
