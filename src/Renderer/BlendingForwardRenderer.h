#pragma once

#include <glm/glm.hpp>

#include "OpenGL.h"

#include "Renderer/DeferredRenderer.h"

#include "API/Camera.h"
#include "SceneTypedefs.h"
#include "Buffers/Framebuffer.h"
#include "Scene/Scene.h"


namespace BlendingForwardRenderer {
    enum AlphaBufferTextureType {
        Accumulated,
        Alpha,
        NumTextures
    };

	inline struct BlendingForwardData {
		std::unique_ptr<CameraReference> renderCamera;

		std::unique_ptr<Framebuffer> alphaBuffer;
		std::unique_ptr<Mesh> quadMesh;

		GLint textureId;
		
		std::unique_ptr<Shader> blendingShader;
		std::unique_ptr<Shader> compositeShader;
	} data;

	inline struct RenderStats {
		size_t drawCalls = 0;
	} stats;
	

	inline void loadShaders() {
		gem::Shader<gem::blending_vertex> bv;
		bv.setblending_vertex_materialData(max_material_count);
		bv.compile();
		gem::Shader<gem::blending_frag> bf;
		bf.setblending_frag_materialData(max_material_count);
		bf.compile();

		data.blendingShader = std::make_unique<Shader>(
			"resources/shaders/build/blending_vertex.glsl", GL_VERTEX_SHADER,
			"resources/shaders/build/blending_frag.glsl", GL_FRAGMENT_SHADER
		);
		data.compositeShader = std::make_unique<Shader>(
			"resources/shaders/blending_composite_vertex.glsl", GL_VERTEX_SHADER,
			"resources/shaders/blending_composite_frag.glsl", GL_FRAGMENT_SHADER
		);
	}

	inline void initialize(const Camera& camera, const GLint textureId, int screenWidth, int screenHeight) {
		data.renderCamera = std::make_unique<CameraReference>(camera);
		data.textureId = textureId;

		FramebufferSpecification alphaBufferSpec{
			screenWidth, screenHeight,
			{{GL_RGBA16F}, {GL_R8}, {GL_DEPTH24_STENCIL8}}
		};
		data.alphaBuffer = std::make_unique<Framebuffer>(alphaBufferSpec);

		// TODO this should be part of internal assets
		data.quadMesh = std::make_unique<Mesh>(GeometryLibrary::get(0), MaterialLibrary::get(0), glm::mat4(1.0));

		loadShaders();
	}
	
	inline void setupFrame() {
		OpenGL::depthMask(false);
		OpenGL::depthTest(true);
		OpenGL::blending(true);
		
		OpenGL::setBlendMode(AlphaBufferTextureType::Accumulated, GL_ONE, GL_ONE);
		OpenGL::setBlendMode(AlphaBufferTextureType::Alpha, GL_ZERO, GL_ONE_MINUS_SRC_COLOR);
		glBlendEquation(GL_FUNC_ADD);

		data.blendingShader->use();
		
		auto view = data.renderCamera->viewMatrix();
		data.blendingShader->setUniform("inverseViewMat", glm::inverse(view)); // glsl inverse is not the same as glm inverse...

		glBindTextureUnit(0, TextureLibrary::defaultTexture->textureID);
		data.blendingShader->setUniform("textureLib", 0);

		data.blendingShader->setUniform("cameraPos", *data.renderCamera->position);
		data.blendingShader->setUniform("lightPos", glm::vec3(5.0));

		data.alphaBuffer->clearColorAttachment(0, 0.0f);
		data.alphaBuffer->clearColorAttachment(1, 1.0f);
 		data.alphaBuffer->clearDepthStencil();

		data.alphaBuffer->bindForWriting();
	}

	inline void render(const Model& model) {
		auto modelMat = model.getModelMatrix();
		glm::mat4 normalMat = glm::mat4(1.0);
		if (model.hasScaleFactor()) {
			normalMat = (model.hasUniformScale() ? modelMat : glm::transpose(glm::inverse(modelMat)));
		}
		data.blendingShader->setUniform("normal", normalMat);

		for (auto mesh : *model.meshes) {
			mesh->bind();
			glDrawElementsInstanced(mesh->type(), mesh->numIndices(), GL_UNSIGNED_INT, nullptr, mesh->instanceCount);
			stats.drawCalls++;
		}
		Mesh::unbind();
	}

	inline void compositePass() {
		OpenGL::depthMask(true);
		OpenGL::setBlendMode(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		Framebuffer::copyDepth(*DeferredRenderer::data.gbuffer, *data.alphaBuffer);

		data.compositeShader->use();

		//glDepthFunc(GL_LESS); // less is the default

		Framebuffer::copyDepthToBackFBO(*DeferredRenderer::data.gbuffer);

		Framebuffer::unbind();
		data.alphaBuffer->bindForReading();
		
		data.alphaBuffer->bindColorTextureUnit(AlphaBufferTextureType::Accumulated);
		data.alphaBuffer->bindColorTextureUnit(AlphaBufferTextureType::Alpha);
		data.compositeShader->setUniform("accum", AlphaBufferTextureType::Accumulated);
		data.compositeShader->setUniform("reveal", AlphaBufferTextureType::Alpha);

		data.quadMesh->bind();
		glDrawElements(data.quadMesh->type(), data.quadMesh->numIndices(), GL_UNSIGNED_INT, nullptr);
		stats.drawCalls++;
		data.quadMesh->unbind();

		data.alphaBuffer->unbind();
	}
	
	inline void endFrame() {
		compositePass();
		OpenGL::blending(false);
	}

	// Utility functions
	
	inline void resetStats() {
		memset(&stats, 0, sizeof(RenderStats));
	}

	inline void resizeFramebuffers(int width, int height) {
		data.alphaBuffer->resize(width, height);
	}

};
