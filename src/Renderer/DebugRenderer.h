#pragma once

#include <glm/glm.hpp>

#include "OpenGL.h"
#include "API/Camera.h"
#include "Buffers/FrameBuffer.h"
#include "Model/Model.h"

#include "Model/Resources/ResourcePack.h"
#include "Scene/Scene.h"
#include "Renderer/Shader.h"

class DebugRenderer {
public:
	inline static std::unique_ptr<Shader> shader;
	inline static std::unique_ptr<Shader> overlayShader;

	inline static std::unique_ptr<Framebuffer> inspectorBuffer;

	inline static struct RenderStats {
		size_t drawCalls = 0;
	} stats;

	inline static glm::vec2 screenMiddle;
	inline static std::unique_ptr<VertexArray> vertexArray;
	inline static VertexBuffer* vertexBuffer;
	inline static IndexBuffer* indexBuffer;

	static void initialize(int screenWidth, int screenHeight) {
		FramebufferSpecification entityBufferSpec{
			screenWidth, screenHeight,
			// materialNumber texture, instance id texture
			{ {GL_R32I}, {GL_R32I}, {GL_DEPTH24_STENCIL8} }
		};
		inspectorBuffer = std::make_unique<Framebuffer>(entityBufferSpec);

		vertexArray = std::make_unique<VertexArray>();
		vertexBuffer = vertexArray->createVertexBuffers(QuadVertex::getFormat())[0];
		indexBuffer = vertexArray->createIndexBuffer();

		screenMiddle = {screenWidth / 2, screenHeight / 2};
		int crosshairSize = 24;
		int crosshairThickness = 2;
		Quad a(screenMiddle.x - (crosshairSize / 2), screenMiddle.y + (crosshairThickness / 2), crosshairSize, crosshairThickness);
		Quad b(screenMiddle.x - (crosshairThickness / 2), screenMiddle.y + (crosshairSize / 2), crosshairThickness, crosshairSize);
		{
			vertexBuffer->reserve(8);
			auto av = a.vertices();
			auto bv = b.vertices();
			vertexBuffer->insertPartial(0, av.data(), 4);
			vertexBuffer->insertPartial(4, bv.data(), 4);

			std::array<GLuint, 12> abi = {0, 1, 2, 2, 3, 0, 4, 5, 6, 6, 7, 4};
			indexBuffer->insert(abi.data(), 12);
		}

		loadShaders();
	}

	static void loadShaders() {
		shader = std::make_unique<Shader>(
			"resources/shaders/debug_vertex.glsl", GL_VERTEX_SHADER,
			"resources/shaders/debug_frag.glsl", GL_FRAGMENT_SHADER
		);
		overlayShader = std::make_unique<Shader>(
			"resources/shaders/text_vertex.glsl", GL_VERTEX_SHADER,
			"resources/shaders/volume_frag.glsl", GL_FRAGMENT_SHADER
		);
	}

	template<size_t SCENE_ID>
	static void render(Scene<SCENE_ID>& scene) {
		shader->use();

		OpenGL::depthTest(true);

		inspectorBuffer->bindForWriting();

		inspectorBuffer->clearColorAttachment(0, -1);
		inspectorBuffer->clearColorAttachment(1, -1);
		inspectorBuffer->clearDepthStencil();
		
		auto modelView = scene.view<PropertiesModel, Meshes, Position3D, Rotation3D, Properties3D>();

		modelView.each([](const PropertiesModel&, const Meshes& meshes, const Position3D& position, const Rotation3D& rotation, const Properties3D& properties3D) {
			/*
			auto modelMat = Object3D::modelMatrix(position, rotation, properties3D);
			shader->setUniform("model", modelMat);
			*/
			for (auto mesh : meshes) {
				mesh->bind();
				glDrawElementsInstanced(mesh->type(), mesh->numIndices(), GL_UNSIGNED_INT, nullptr, mesh->instanceCount);
				stats.drawCalls++;
			}
			Mesh::unbind();
		});

		Framebuffer::unbind();

		OpenGL::depthTest(false);
		overlayShader->use();
		vertexArray->bind();
		glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, nullptr);
		VertexArray::unbind();
		OpenGL::depthTest(true);
	}
	
	// Utility functions
	
	static void resetStats() {
		memset(&stats, 0, sizeof(RenderStats));
	}

};
