#pragma once

#include "Renderer/Buffers/UniformBuffer.h"

#include "VFX/LightTypes.h"
#include "Model/Resources/MaterialLibrary.h"
#include "Renderer/ShaderProgram.h"

class DeferredPointLightTechnique {
public:
	using vertUnis = Binder::deferred_point_vertex::uniforms;
	using fragUnis = Binder::deferred_point_frag::uniforms;
	
	static const Shader& shader() { return ShaderProgram::deferredPointShader(); }


	static void setPointLightIndex(const int index) {
		shader().setUniform(fragUnis::pointLightIndex, index);
	}

	static void setModel(const glm::dmat4& model) {
		shader().setUniform(vertUnis::model, 1, GL_FALSE, model);
	}

	static void setView(const glm::dmat4& view) {
		shader().setUniform(vertUnis::view, 1, GL_FALSE, view);
	}

	static void setWorldCameraPos(const glm::vec3& cameraPos) {
		shader().setUniform(fragUnis::worldCameraPos, 1, cameraPos);
	}

	static void setCameraViewMat(const glm::mat4& view) {
		shader().setUniform(fragUnis::cameraViewMat, 1, GL_FALSE, view);

		//setInverseViewMat(glm::inverse(view));
	}

	static void uploadMaterialData() {
		auto& materialData = getUBMaterialData();
		UniformBuffer::insert(materialData, MaterialLibrary::getMaterialData());
	}

	static void uploadSpotLightData(const std::vector<SpotLight>& lightVector) {
		auto& spotLightBuffer = getUBSpotLightData();
		UniformBuffer::insert(spotLightBuffer, lightVector);
	}

	static void uploadPointLightData(const std::vector<PointLight>& lightVector) {
		auto& pointLightBuffer = getUBPointLightData();
		UniformBuffer::insert(pointLightBuffer, lightVector);
	}

	static void setFogColor(const glm::vec3& color) {
		shader().setUniform(fragUnis::fogColor, 1, color);
	}

	static UniformBuffer& getUBMaterialData() {
		static UniformBuffer materialData = UniformBuffer(Binder::deferred_point_frag::buffers::MaterialData);
		return materialData;
	}

	static UniformBuffer& getUBSpotLightData() {
		static UniformBuffer spotLightData = UniformBuffer(Binder::deferred_point_frag::buffers::SpotLightData);
		return spotLightData;
	}

	static UniformBuffer& getUBPointLightData() {
		static UniformBuffer pointLightData = UniformBuffer(Binder::deferred_point_frag::buffers::PointLightData);
		return pointLightData;
	}

};

