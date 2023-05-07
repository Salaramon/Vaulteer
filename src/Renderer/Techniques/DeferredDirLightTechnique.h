#pragma once

#include "Model/Material.h"
#include "Renderer/ShaderProgram.h"
#include "Renderer/Buffers/UniformBuffer.h"

#include "VFX/LightTypes.h"

class DeferredDirLightTechnique {
public:
	using dirVertUnis = Binder::deferred_directional_vertex::uniforms;
	using dirFragUnis = Binder::deferred_directional_frag::uniforms;

	static const Shader& shader() { return ShaderProgram::deferredDirShader(); }

	static void setWorldCameraPos(const glm::vec3& cameraPos) {
	    shader().setUniform(dirFragUnis::worldCameraPos, 1, cameraPos);
	}

	static void setCameraViewMat(const glm::mat4& view) {
	    shader().setUniform(dirFragUnis::cameraViewMat, 1, GL_FALSE, view);
	}

	static void setFogColor(const glm::vec3& color) {
	    shader().setUniform(dirFragUnis::fogColor, 1, color);
	}


	// uniform buffer upload
	// TODO should be handled elsewhere that can guarantee their state or else they're copied between a bunch of techniques

	static void uploadMaterialData() {
		auto& materialData = getUBMaterialData();
		UniformBuffer::insert(materialData, MaterialLibrary::getMaterialData());
	}

	static void uploadDirectionalLightData(const std::vector<DirectionalLight>& lightVector) {
		auto& dirLightBuffer = getUBDirectionalLightData();
		UniformBuffer::insert(dirLightBuffer, lightVector);
	}



	static UniformBuffer& getUBMaterialData() {
		static auto materialData = UniformBuffer(Binder::deferred_directional_frag::buffers::MaterialData);
		return materialData;
	}

	static UniformBuffer& getUBDirectionalLightData() {
		static auto dirLightData = UniformBuffer(Binder::deferred_directional_frag::buffers::DirectionalLightData);
		return dirLightData;
	}

};

