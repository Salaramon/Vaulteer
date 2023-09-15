#pragma once

#include <unordered_map>
#include <string>
#include <ranges>

#include "Model/Textures/Texture2DArray.h"

constexpr int validTextureTypeSize = 3;



class Material {
public:
	std::string name;
	unsigned int materialIndex = -1;	// populated and used by material library
	
	std::unordered_map<aiTextureType, TextureResourceLocator> textureTypeLocators;

	// layout used in uniform buffers
	struct alignas(16) MaterialData {
		glsl_vec3 colorAmbient, colorDiffuse, colorSpecular;
		float matShininess, matOpacity;
		int textureId;
	} data;

	/*
	// flags
	bool twoSided = false;
	bool doDepthTest = true;

	bool hasNormalMap = false;
	bool hasSpecularMap = false;

	//bool transparent = false;
	bool shadow = false;
	bool blur = false;*/


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

	bool hasSpecularMap() const {
		return textureTypeLocators.contains(aiTextureType_SPECULAR);
	}

	bool hasNormalMap() const {
		return textureTypeLocators.contains(aiTextureType_NORMALS);
	}

	TextureResourceLocator getLocator(aiTextureType type) {
		return textureTypeLocators.at(type);
	}


	void setTextureId(GLint textureId) {
		this->data.textureId = textureId;
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

	
	inline static std::array<aiTextureType, validTextureTypeSize> validTextureTypes {
		aiTextureType_DIFFUSE,
		aiTextureType_SPECULAR,
		aiTextureType_HEIGHT,
	};

	inline static MaterialData defaultMaterial = {
		.colorAmbient = glm::vec3(0.0f),
		.colorDiffuse = glm::vec3(0.6f),
		.colorSpecular = glm::vec3(1.0f),
		.matShininess = 32.0f, .matOpacity = 1.0f,
		.textureId = 0
	};
};