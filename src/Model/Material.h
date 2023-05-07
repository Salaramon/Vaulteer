#pragma once

#include <unordered_map>
#include <string>
#include <ranges>

#include "Model/Textures/Texture2DArray.h"

class Material {
public:
	std::string name;
	unsigned int materialIndex = -1;	// populated and used by material library

	// layout used in uniform buffers
	struct alignas(16) MaterialData {
		glsl_vec3 colorAmbient, colorDiffuse, colorSpecular;
		float matShininess, matOpacity;
		int textureId;
	} data;
	
	std::unordered_map<aiTextureType, TextureResourceLocator> textureTypeLocators;

	// flags
	bool twoSided = false;
	bool doDepthTest = true;

	bool hasNormalMap = false;
	bool hasSpecularMap = false;

	//bool transparent = false;
	bool shadow = false;
	bool blur = false;


	Material() : name("DefaultMaterial"), data(defaultMaterial) {}

	Material(const std::string& name, const MaterialData& materialData) : name(name) {
		if (name == "DefaultMaterial") {
			data = defaultMaterial; // we override ASSIMP's default with our own
			return;
		}
		data = materialData;
	}

	bool isTransparent() const {
		return data.matOpacity < 1.0f;
	}

	TextureResourceLocator getLocator(aiTextureType type) {
		return textureTypeLocators.at(type);
	}


	void setMaterialIndex(unsigned int index) {
		this->materialIndex = index;
		// update locators
		for (auto& val : textureTypeLocators | std::views::values)
			val.materialIndex = index;
	}

	void putTextureLocator(aiTextureType type, const TextureResourceLocator& loc) {
		textureTypeLocators[type] = loc;
	}


private:
	inline static MaterialData defaultMaterial = {
		.colorAmbient = glm::vec3(0.0f),
		.colorDiffuse = glm::vec3(0.6f),
		.colorSpecular = glm::vec3(1.0f),
		.matShininess = 32.0f, .matOpacity = 1.0f,
		.textureId = 0
	};
};