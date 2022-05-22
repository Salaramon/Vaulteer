#pragma once

#include <unordered_map>

#include <assimp/scene.h>

#include "Texture2DArray.h"

//using Material = std::unordered_map<aiTextureType, Texture2DArray::TextureResourceLocator>;

class Material {
public:
	std::string name;

	glm::vec3 colorAmbient, colorDiffuse, colorSpecular;

	float matShininess, matOpacity;

	std::unordered_map<aiTextureType, Texture2DArray::TextureResourceLocator> textureTypeLocators;

	bool isTransparent() {
		return matOpacity < 1.0;
	}

	Material() : 
		name("DefaultMaterial"), 
		colorAmbient({0.0}), 
		colorDiffuse({0.6}), 
		colorSpecular({0.0}), 
		matShininess(32.0f),
		matOpacity(1.0f) 
	{}

	Material(aiMaterial* mat, std::string folderPath) {
		aiString aiName;
		mat->Get(AI_MATKEY_NAME, aiName);
		name = std::string(aiName.C_Str());

		mat->Get(AI_MATKEY_SHININESS, matShininess);
		mat->Get(AI_MATKEY_OPACITY, matOpacity);

		aiColor3D color;
		mat->Get(AI_MATKEY_COLOR_AMBIENT, color);
		colorAmbient = aiCol_glmVec(color);
		mat->Get(AI_MATKEY_COLOR_DIFFUSE, color);
		colorDiffuse = aiCol_glmVec(color);
		mat->Get(AI_MATKEY_COLOR_SPECULAR, color);
		colorSpecular = aiCol_glmVec(color);
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

private:
	glm::vec3 aiCol_glmVec(aiColor3D aiColor) {
		return { aiColor.r, aiColor.g, aiColor.b };
	}
};