#pragma once

#include <unordered_map>
#include <string>
#include <ranges>

#include "Model/Textures/Texture2DArray.h"

class Material {
public:
	std::string name;
	unsigned int materialIndex;			 // populated and used by material library
	Texture2DArray::TextureUnit texUnit; // populated on resource pack finalization

	// layout used in uniform buffers
	struct alignas(16) MaterialData {
		glsl_vec3 colorAmbient, colorDiffuse, colorSpecular;
		float matShininess, matOpacity;
	} data;

	std::unordered_map<aiTextureType, TextureResourceLocator> textureTypeLocators;

	// flags
	bool twoSided;
	bool doDepthTest;

	bool hasNormalMap;
	bool hasSpecularMap;

	//bool emissiveLight; ?
	//bool reflectiveLight; ?

	bool transparent;
	bool shadow;
	bool blur;


	Material() :
		name("DefaultMaterial"), data(defaultMaterial) {}



	Material(aiMaterial* mat, const std::string& objPath) {
		aiString aiName;
		mat->Get(AI_MATKEY_NAME, aiName);
		name = std::string(aiName.C_Str());

		if (name == "DefaultMaterial") {
			data = defaultMaterial; // we override ASSIMP's default with our own
			return;
		}

		mat->Get(AI_MATKEY_SHININESS, data.matShininess);
		mat->Get(AI_MATKEY_OPACITY, data.matOpacity);

		aiColor3D color;
		mat->Get(AI_MATKEY_COLOR_AMBIENT, color);
		data.colorAmbient = aiCol_glmVec(color);
		mat->Get(AI_MATKEY_COLOR_DIFFUSE, color);
		data.colorDiffuse = aiCol_glmVec(color);
		mat->Get(AI_MATKEY_COLOR_SPECULAR, color);
		data.colorSpecular = aiCol_glmVec(color);
		mat->Get(AI_MATKEY_COLOR_EMISSIVE, color);
		mat->Get(AI_MATKEY_COLOR_REFLECTIVE, color);
		mat->Get(AI_MATKEY_COLOR_TRANSPARENT, color);

		createTextureLocators(mat, objPath);
	}

	bool isTransparent() const {
		return data.matOpacity < 1.0f;
	}

	void setMaterialIndex(unsigned int materialIndex) {
		this->materialIndex = materialIndex;
		// update locators
		for (auto& val : textureTypeLocators | std::views::values)
			val.materialIndex = materialIndex;
	}

private:
	inline static MaterialData defaultMaterial = {
		.colorAmbient = glm::vec3(0.0f),
		.colorDiffuse = glm::vec3(0.6f),
		.colorSpecular = glm::vec3(1.0f),
		.matShininess = 32.0f,
		.matOpacity = 1.0f,
	};

	inline void createTextureLocators(const aiMaterial* mat, const std::string& objPath) {
		size_t index = objPath.find_last_of('/');
		std::string folderPath = index != std::string::npos ? objPath.substr(0, index) + "/" : ".";

		for (size_t i = static_cast<size_t>(aiTextureType_NONE); i < static_cast<size_t>(aiTextureType_UNKNOWN) + 1; i++) {
			aiTextureType type = static_cast<aiTextureType>(i);

			for (size_t j = 0; j < mat->GetTextureCount(type); j++) {
				aiString string;
				mat->GetTexture(type, j, &string);
				TextureResourceLocator loc = { std::string(folderPath + string.C_Str()), type };
				textureTypeLocators[type] = loc;
			}
		}
	}

	glm::vec3 aiCol_glmVec(aiColor3D aiColor) {
		return { aiColor.r, aiColor.g, aiColor.b };
	}
};