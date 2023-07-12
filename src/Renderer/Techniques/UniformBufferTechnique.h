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
	inline static std::unordered_map<std::string, UniformBlock> uniformBlockCache;
	inline static std::unordered_map<GLuint, UniformBlock*> conflicts;

	inline static std::unordered_map<GLint, std::unique_ptr<UniformBuffer>> buffers;

	static GLint binding(const std::string& name) {
		auto it = uniformBlockCache.find(name);
		if (it != uniformBlockCache.end()) {
			return it->second.binding;
		}
		return -1;
	}

	static void put(const GLuint shaderProgram, const std::string& name, const UniformBlock& block) {
		if (discoverConflicts(shaderProgram, name, block))
			return;

		uniformBlockCache.emplace(name, block);
		buffers.emplace(block.binding, std::make_unique<UniformBuffer>(block));
	}

	
	static void uploadCameraView(const glm::mat4& view) {
		buffers.at(binding("Camera"))->insertAt(view, 0);
	}
	static void uploadCameraProjection(const glm::mat4& projection) {
		buffers.at(binding("Camera"))->insertAt(projection, sizeof(glm::mat4));
	}
	static void uploadScreenProjection(const glm::mat4& projection) {
		buffers.at(binding("Camera"))->insertAt(projection, sizeof(glm::mat4) * 2);
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


	static bool discoverConflicts(const GLuint shaderProgram, const std::string& name, const UniformBlock& block) {
		auto it = uniformBlockCache.find(name);
		if (it != uniformBlockCache.end()) {
			if (block.binding != it->second.binding) {
				conflicts[block.binding] = &it->second;

				std::string error = std::format("conflict in UBO definitions - {} (binding {}) does not match {} (binding {}) in program {}",
					block.name, block.binding,
					it->second.name, it->second.binding,
					shaderProgram);
				KYSE_ASSERT(false, error);
			}
			return true;
		}
		auto bufIt = buffers.find(block.binding);
		if (bufIt != buffers.end() && block.name.compare(buffers[block.binding]->name) != 0) {
			auto conflicting = buffers[block.binding].get();

			std::string error = std::format("conflict in UBO definitions - {} (binding {}) already bound for {} (binding {}), currently reading program {}",
				block.name, block.binding,
				conflicting->name, conflicting->binding,
				shaderProgram
			);
			KYSE_ASSERT(false, error);

			return true;
		}
		return false;
	}

};
