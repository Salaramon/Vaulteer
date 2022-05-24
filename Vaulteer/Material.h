#pragma once

#include <assimp/scene.h>
#include <unordered_map>

#include "Texture2DArray.h"
#include "TypeDefUtils.h"

//using Material = std::unordered_map<aiTextureType, Texture2DArray::TextureResourceLocator>;

class Material {
public:
	std::string name;

	// layout used in uniform buffers
	struct alignas(16) MaterialData {
		glsl_vec3 colorAmbient, colorDiffuse, colorSpecular;
		float matShininess, matOpacity;
	} data;

	std::unordered_map<aiTextureType, Texture2DArray::TextureResourceLocator> textureTypeLocators;


	bool isTransparent() {
		return data.matOpacity < 1.0;
	}

	Material() : 
		name("DefaultMaterial"), data(defaultMaterial) {
	}

	Material(aiMaterial* mat, std::string folderPath) {
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

		createTexture(mat, folderPath);
	}

	void createTexture(aiMaterial* mat, std::string folderPath) {
		for (size_t i = static_cast<size_t>(aiTextureType_NONE); i < static_cast<size_t>(aiTextureType_UNKNOWN) + 1; i++) {
			aiTextureType type = static_cast<aiTextureType>(i);

			for (size_t j = 0; j < mat->GetTextureCount(type); j++) {
				aiString string;
				mat->GetTexture(type, j, &string);
				Texture2DArray::TextureResourceLocator loc = { std::string(folderPath + string.C_Str()), type };
				textureTypeLocators[type] = loc;
			}
		}
	}

	inline static MaterialData defaultMaterial = {
		.colorAmbient = glm::vec3(0.0f),
		.colorDiffuse = glm::vec3(0.6f),
		.colorSpecular = glm::vec3(1.0f),
		.matShininess = 32.0f,
		.matOpacity = 1.0f,
	};

private:
	glm::vec3 aiCol_glmVec(aiColor3D aiColor) {
		return { aiColor.r, aiColor.g, aiColor.b };
	}
};