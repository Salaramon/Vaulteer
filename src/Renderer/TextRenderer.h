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

	inline static Font* monospaceFont;
	inline static Font* regularFont;
	inline static glm::vec2 screenMiddle;

	inline static constexpr size_t maxQuads = 1000;
	inline static std::array<QuadVertex, maxQuads * 4> vertices;
	inline static std::array<GLuint, maxQuads * 6> indices;

	inline static struct RenderStats {
		size_t numQuads = 0;
		size_t drawCalls = 0;

		size_t numVertices() {
			return numQuads * 4;
		}

		size_t numIndices() {
			return numQuads * 6;
		}
	} stats;


	static void initialize(uint screenWidth, uint screenHeight) {
		buildScreenProjection(screenWidth, screenHeight);

		vertexArray = std::make_unique<VertexArray>();
		vertexBuffer = vertexArray->createVertexBuffers(QuadVertex::getFormat()).at(0);
		indexBuffer = vertexArray->createIndexBuffer();

		vertexBuffer->reserve(maxQuads * 4);
		indexBuffer->reserve(maxQuads * 6);

		monospaceFont = FontLibrary::loadFont({"Hack", "resources/fonts/hack/msdf.json", "resources/fonts/hack/msdf.png"});
		regularFont = FontLibrary::loadFont({"Georgia", "resources/fonts/georgia/msdf.json", "resources/fonts/georgia/msdf.png"});

		screenMiddle = {screenWidth / 2, screenHeight / 2};

		loadShaders();
	}

	static void loadShaders() {
		shader = std::make_unique<Shader>(
			"resources/shaders/text_vertex.glsl", GL_VERTEX_SHADER,
			"resources/shaders/text_frag.glsl", GL_FRAGMENT_SHADER
		);
	}


	template <size_t SCENE_ID>
	static void render(Scene<SCENE_ID>& scene) {
		flush();
	}

	static void flush() {
		if (stats.numQuads) {
			shader->use();

			OpenGL::depthTest(false);
			OpenGL::blending(true);
			OpenGL::setBlendMode(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			glBindTextureUnit(0, regularFont->mapTexture->textureID);
			shader->setUniform("msdf", 0);

			vertexArray->bind();
			glDrawElements(GL_TRIANGLES, stats.numIndices(), GL_UNSIGNED_INT, nullptr);
			vertexArray->unbind();

			OpenGL::blending(false);
			OpenGL::depthTest(true);

			stats.drawCalls++;
		}
	}

	static void clearScene() {
		stats.numQuads = 0;
	}

	static void submitText(std::string content, glm::vec2 position = screenMiddle, float scale = 1.0, Justify justify = Justify::Left) {

		//TODO make some eventual indirection of quad generation... should be handled by callers
		auto [lineSize, quads] = FontLibrary::stringToQuads(regularFont, content, position, scale, justify);

		uint offset = stats.numQuads;
		uint index = 0;
		for (auto& quad : quads) {

			if (stats.numQuads >= maxQuads) {
				if (index > 0) {
					vertexBuffer->insertPartial(offset * 4, &vertices[offset * 4], index * 4);
					indexBuffer->insertPartial(offset * 6, &indices[offset * 6], index * 6);
				}

				flush();
				clearScene();

				offset = 0;
				index = 0;
			}

			std::array<QuadVertex, 4> v = quad.vertices();
			std::copy(std::begin(v), std::end(v), vertices.begin() + stats.numVertices());

			indices[stats.numIndices() + 0] = (index + offset) * 4 + 0;
			indices[stats.numIndices() + 1] = (index + offset) * 4 + 1;
			indices[stats.numIndices() + 2] = (index + offset) * 4 + 2;
			indices[stats.numIndices() + 3] = (index + offset) * 4 + 2;
			indices[stats.numIndices() + 4] = (index + offset) * 4 + 3;
			indices[stats.numIndices() + 5] = (index + offset) * 4 + 0;

			stats.numQuads++;
			index++;
		}

		vertexBuffer->insertPartial(offset * 4, &vertices[offset * 4], index * 4);
		indexBuffer->insertPartial(offset * 6, &indices[offset * 6], index * 6);
	}


	static void buildScreenProjection(uint screenWidth, uint screenHeight) {
		projection = getScreenProjectionMatrix(screenWidth, screenHeight);
		UniformBufferTechnique::uploadScreenProjection(projection);
	}

	// orthographic projection matrix with size of screen
	static glm::mat4 getScreenProjectionMatrix(uint w, uint h) {
		return {
			2.f / w, 0, 0, 0,
			0, -2.f / h, 0, 0,
			0, 0, -1, 0,
			-1, 1, 0, 1
		};
	}

	static void resetStats() {
		memset(&stats, 0, sizeof(RenderStats));
	}
};
