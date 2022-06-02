#pragma once

#include <assimp/scene.h>
#include <unordered_map>

#include "Model/Textures/Texture2DArray.h"
#include "Utils/TypeDefUtils.h"

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

	// flags
	// bool twoSided;
	// bool doDepthTest;


	Material();

	Material(aiMaterial* mat, std::string folderPath);

	bool isTransparent();

private:
	inline static MaterialData defaultMaterial = {
		.colorAmbient = glm::vec3(0.0f),
		.colorDiffuse = glm::vec3(0.6f),
		.colorSpecular = glm::vec3(1.0f),
		.matShininess = 32.0f,
		.matOpacity = 1.0f,
	};

	inline void createTexture(aiMaterial* mat, std::string folderPath);

	glm::vec3 aiCol_glmVec(aiColor3D aiColor);
};