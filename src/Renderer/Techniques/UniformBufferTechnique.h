#pragma once

#include <string>
#include <unordered_map>
#include <glad/glad.h>

#include "Model/Resources/MaterialLibrary.h"
#include "Model/Resources/TextureLibrary.h"
#include "Renderer/Buffers/UniformBuffer.h"
#include "VFX/LightTypes.h"


class UniformBufferTechnique {
public:
	struct UniformBlock {
		std::string name;
		GLuint binding;
		GLsizei size;

		GLuint programSource;
	};

	inline static std::unordered_map<std::string, UniformBlock> uniformBufferCache;
	inline static std::unordered_map<GLuint, UniformBlock*> conflicts;

	inline static std::vector<std::unique_ptr<UniformBuffer>> buffers;

	static GLuint binding(const std::string& name) {
		auto it = uniformBufferCache.find(name);
		if (it != uniformBufferCache.end()) {
			return it->second.binding;
		}
		return -1;
	}

	static void put(const GLuint shaderProgram, const std::string& name, const UniformBlock& block) {
		auto it = uniformBufferCache.find(name);
		if (it != uniformBufferCache.end()) {
			if (block.binding != it->second.binding) {
				conflicts[block.binding] = &it->second;

				std::cout << std::format("conflict in UBO definitions - {} (binding {}) does not match {} (binding {}) in program {}",
					block.name, block.binding,
					it->second.name, it->second.binding,
					shaderProgram
				) << std::endl;
				__debugbreak();
			}
			return;
		}

		uniformBufferCache.emplace(name, block);
		buffers.emplace_back(std::make_unique<UniformBuffer>(block.binding, block.size));
	}

	
	static void uploadCameraProjection(const glm::mat4& projection) {
		buffers.at(binding("Camera"))->insert(projection);
	}
	
	static void uploadMaterialData() {
		buffers.at(binding("MaterialData"))->insert(MaterialLibrary::getMaterialData());
	}

	static void uploadTextureData() {
		buffers.at(binding("TextureData"))->insert(TextureLibrary::getTextureData());
	}

	static void uploadTextureViewData() {
		buffers.at(binding("TextureViewData"))->insert(TextureLibrary::getTextureViewData());
	}
	
	static void uploadDirectionalLightData(const std::vector<DirectionalLight>& dirLights) {
		buffers.at(binding("DirectionalLightData"))->insert(dirLights);
	}
	
	static void uploadPointLightData(const std::vector<PointLight>& pointLights) {
		buffers.at(binding("PointLightData"))->insert(pointLights);
	}

};
