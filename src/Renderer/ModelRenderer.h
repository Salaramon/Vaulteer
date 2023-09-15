#pragma once
#include "Shader.h"
#include "Model/Mesh.h"

#include "OpenGL.h"
#include "API/Camera.h"
#include "Model/Model.h"




struct ModelRenderer {
	ShaderType currentType;

	void render(const Model& model) {
		// could separate this further by making shader types for instancing... or anything else
		ShaderType modelShaderType = model.meshes->at(0)->shaderType;
		
		if (currentType != modelShaderType) {
			endFrame();
		}

		switch (modelShaderType) {
		case ShaderType::Forward:
			if (currentType != ShaderType::Forward) {
				ForwardRenderer::setupFrame();
			}
			ForwardRenderer::render(model);
			break;

		case ShaderType::Transparent:
			if (currentType != ShaderType::Transparent) {
				BlendingForwardRenderer::setupFrame();
			}
			BlendingForwardRenderer::render(model);
			break;

		case ShaderType::Deferred:
			if (currentType != ShaderType::Deferred) {
				DeferredRenderer::setupFrame();
			}
			DeferredRenderer::render(model);
			break;
			
		case ShaderType::Deferred_NormalSpecular: // unimplemented

		default: 
			__debugbreak();
			break;
		}
		currentType = modelShaderType;
	}

	void endFrame() {
		switch(currentType) {
		case ShaderType::Forward:
			ForwardRenderer::endFrame();
			break;
		case ShaderType::Transparent:
			BlendingForwardRenderer::endFrame();
			break;
			
		case ShaderType::Deferred:
		case ShaderType::Deferred_NormalSpecular:
			DeferredRenderer::endFrame();
			break;

		default:
			__debugbreak();
		case ShaderType::None:
			break;
		}
	}


	void reloadShaders() {
		ForwardRenderer::loadShaders();
		BlendingForwardRenderer::loadShaders();
		DeferredRenderer::loadShaders();
		TextRenderer::loadShaders();
	}

	size_t getNumDrawCalls() {
		return
		ForwardRenderer::stats.drawCalls +
		BlendingForwardRenderer::stats.drawCalls +
		DeferredRenderer::stats.drawCalls +
		TextRenderer::stats.drawCalls;
	}

	void resetStats() {
		memset(&ForwardRenderer::stats, 0, sizeof(ForwardRenderer::RenderStats));
		memset(&BlendingForwardRenderer::stats, 0, sizeof(BlendingForwardRenderer::RenderStats));
		memset(&DeferredRenderer::stats, 0, sizeof(DeferredRenderer::RenderStats));
		memset(&TextRenderer::stats, 0, sizeof(TextRenderer::RenderStats));
	}

};
