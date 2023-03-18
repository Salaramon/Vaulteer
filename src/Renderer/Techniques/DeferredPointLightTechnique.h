#pragma once

#include "Renderer/Buffers/UniformBuffer.h"

#include "VFX/LightTypes.h"

class DeferredPointLightTechnique {
public:
	using vertUnis = Binder::deferred_point_vertex::uniforms;
	using fragUnis = Binder::deferred_point_frag::uniforms;
	static void init();

	static void setModel(const glm::dmat4& model);
	static void setView(const glm::dmat4& view);

	static void setPointLightIndex(int index);

	/*static void setDirectionalLight(const DirectionalLight& dirLight);
	static void setSpotLight(const SpotLight& light, int index);*/

	static void setWorldCameraPos(const glm::vec3& cameraPos);
	static void setCameraViewMat(const glm::mat4& view);

	static void setFogColor(const glm::vec3& color);

	// uniform buffer upload
	static void uploadMaterialData();
	static void uploadPointLightData(const std::vector<PointLight>& lightVector);
	static void uploadSpotLightData(const std::vector<SpotLight>& lightVector);

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

