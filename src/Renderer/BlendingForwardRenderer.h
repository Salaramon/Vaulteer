#pragma once

#include <glm/glm.hpp>

#include "OpenGL.h"

#include "Renderer/DeferredRenderer.h"

#include "API/Camera.h"
#include "SceneTypedefs.h"
#include "Buffers/Framebuffer.h"
#include "Scene/Scene.h"


class BlendingForwardRenderer {
    enum AlphaBufferTextureType {
        Accumulated,
        Alpha,
        NumTextures
    };
	inline static std::unique_ptr<Framebuffer> alphaBuffer;
	inline static Mesh* quadMesh;

	inline static GLint textureId;
	
	inline static std::unique_ptr<Shader> blendingShader;
	inline static std::unique_ptr<Shader> compositeShader;

public:
	inline static struct RenderStats {
		size_t drawCalls = 0;
	} stats;


	static void initialize(const GLint textureId, int screenWidth, int screenHeight) {
		BlendingForwardRenderer::textureId = textureId;

		FramebufferSpecification alphaBufferSpec{
			screenWidth, screenHeight,
			{{GL_RGBA16F}, {GL_R8}, {GL_DEPTH24_STENCIL8}}
		};
		alphaBuffer = std::make_unique<Framebuffer>(alphaBufferSpec);

		quadMesh = ResourceLoader::importModel("resources/quad.obj")[0];

		loadShaders();
	}

	static void loadShaders() {
		gem::Shader<gem::blending_vertex> bv;
		bv.setblending_vertex_materialData(max_material_count);
		bv.compile();
		gem::Shader<gem::blending_frag> bf;
		bf.setblending_frag_materialData(max_material_count);
		bf.compile();

		blendingShader = std::make_unique<Shader>(
			"resources/shaders/build/blending_vertex.glsl", GL_VERTEX_SHADER,
			"resources/shaders/build/blending_frag.glsl", GL_FRAGMENT_SHADER
		);
		compositeShader = std::make_unique<Shader>(
			"resources/shaders/blending_composite_vertex.glsl", GL_VERTEX_SHADER,
			"resources/shaders/blending_composite_frag.glsl", GL_FRAGMENT_SHADER
		);
	}

	template<size_t SCENE_ID>
	static void render(Scene<SCENE_ID>& scene) {
		OpenGL::depthMask(false);
		OpenGL::depthTest(true);
		OpenGL::blending(true);

		OpenGL::setBlendMode(AlphaBufferTextureType::Accumulated, GLBlendModes::One, GLBlendModes::One);
		OpenGL::setBlendMode(AlphaBufferTextureType::Alpha, GLBlendModes::Zero, GLBlendModes::OneMinusSourceColor);
		glBlendEquation(GL_FUNC_ADD);

		blendingPass(scene);
			
		OpenGL::depthMask(true);
		OpenGL::setBlendMode(GLBlendModes::SourceAlpha, GLBlendModes::OneMinusSourceAlpha);

		Framebuffer::copyDepth(*DeferredRenderer::gbuffer, *alphaBuffer);
		Framebuffer::unbind();
		
		compositePass();
		
		OpenGL::blending(false);
	}

	template<size_t SCENE_ID>
	static void blendingPass(Scene<SCENE_ID>& scene) {
		blendingShader->use();
		
		auto camera = scene.getActiveCamera();
		auto modelView = scene.view<PropertiesModel, Meshes, Position3D, Rotation3D, Properties3D, Transparent>();

		auto view = camera.viewMatrix();
		blendingShader->setUniform("inverseViewMat", glm::inverse(view)); // glsl inverse is not the same as glm inverse...

		glBindTextureUnit(0, TextureLibrary::defaultTexture->textureID);
		blendingShader->setUniform("textureLib", 0);

		blendingShader->setUniform("cameraPos", *camera.position);
		blendingShader->setUniform("lightPos", glm::vec3(5.0));

		alphaBuffer->clearColorAttachment(0, 0.0f);
		alphaBuffer->clearColorAttachment(1, 1.0f);
 		alphaBuffer->clearDepthStencil();

		alphaBuffer->bindForWriting();
		
		modelView.each([](const PropertiesModel&, const Meshes& meshes, const Position3D& position, const Rotation3D& rotation, const Properties3D& properties3D, const Transparent&) {
			auto modelMat = Object3D::modelMatrix(position, rotation, properties3D);

			blendingShader->setUniform("model", modelMat);
			blendingShader->setUniform("normal", glm::transpose(glm::inverse(modelMat)));

			for (auto mesh : meshes) {
				mesh->bind();
				glDrawElements(mesh->getType(), mesh->getNumIndices(), GL_UNSIGNED_INT, nullptr);
				stats.drawCalls++;
			}
			Mesh::unbind();
		});

		alphaBuffer->unbind();
	}

	static void compositePass() {
		compositeShader->use();

		//glDepthFunc(GL_LESS); // less is the default

		Framebuffer::copyDepthToBackFBO(*DeferredRenderer::gbuffer);

		alphaBuffer->bindForReading();
		
		alphaBuffer->bindColorTextureUnit(AlphaBufferTextureType::Accumulated);
		alphaBuffer->bindColorTextureUnit(AlphaBufferTextureType::Alpha);
		compositeShader->setUniform("accum", AlphaBufferTextureType::Accumulated);
		compositeShader->setUniform("reveal", AlphaBufferTextureType::Alpha);

		quadMesh->bind();
		glDrawElements(GL_TRIANGLES, quadMesh->getNumIndices(), GL_UNSIGNED_INT, nullptr);
		stats.drawCalls++;
		quadMesh->unbind();

		alphaBuffer->unbind();
	}

	// Utility functions
	
	static void resetStats() {
		memset(&stats, 0, sizeof(RenderStats));
	}

	static void resizeFramebuffers(int width, int height) {
		alphaBuffer->resize(width, height);
	}

};
