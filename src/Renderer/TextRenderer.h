#pragma once

#include <algorithm>

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

	inline static glm::mat4 projection;

	inline static std::unique_ptr<Shader> shader;

	inline static Font* font;
	inline static glm::vec2 screenMiddle;

	inline static constexpr uint maxQuads = 1000;
	inline static std::array<QuadVertex, maxQuads * 4> vertices;
	inline static std::array<GLuint, maxQuads * 6> indices;

	inline static uint numQuads = 0;
	inline static uint numVertices = 0, numIndices = 0;

	static void initialize(uint screenWidth, uint screenHeight) {
		buildScreenProjection(screenWidth, screenHeight);

		vertexArray = std::make_unique<VertexArray>();
		vertexBuffer = vertexArray->createVertexBuffers(QuadVertex::getFormat()).at(0);
		indexBuffer = vertexArray->createIndexBuffer();

		vertexBuffer->reserve(maxQuads * 4);
		indexBuffer->reserve(maxQuads * 6);

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

		OpenGL::depthTest(false);
		OpenGL::blending(true);
		OpenGL::setBlendMode(GLBlendModes::SourceAlpha, GLBlendModes::One);

		glBindTextureUnit(0, font->mapTexture->textureID);
		shader->setUniform("msdf", 0);

		vertexArray->bind();
		glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, nullptr);
		vertexArray->unbind();
		
		OpenGL::blending(false);
		OpenGL::depthTest(true);
	}

	static void clearScene() {
		numQuads = 0;
		numVertices = 0;
		numIndices = 0;
	}

	static void submitText(std::string content, glm::vec2 position = screenMiddle, float scale = 1.0) {

		uint index = numQuads;
		auto [lineSize, quads] = FontLibrary::stringToQuads(font, content, position, scale);
		numQuads += quads.size();

		int offset = 0;
		for (auto& quad : quads) {
			/* eventual quad transforms here
			quad.x -= lineSize.x / 2;
			quad.y -= lineSize.y / 2;
			*/

			std::array<QuadVertex, 4> v = quad.vertices();
			std::copy(std::begin(v), std::end(v), vertices.begin() + numVertices);
			numVertices += 4;

			std::array<uint, 6> in = quad.indices();
			indices[numIndices + 0] = (offset + index) * 4 + 0;
			indices[numIndices + 1] = (offset + index) * 4 + 1;
			indices[numIndices + 2] = (offset + index) * 4 + 2;
			indices[numIndices + 3] = (offset + index) * 4 + 2;
			indices[numIndices + 4] = (offset + index) * 4 + 3;
			indices[numIndices + 5] = (offset + index) * 4 + 0;
			numIndices += 6;
			offset++;
		}

		vertexBuffer->insertPartial(index * 4, &vertices[index*4], offset * 4);
		indexBuffer->insertPartial(index * 6, &indices[index*6], offset * 6);
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
