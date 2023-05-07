#pragma once

#include "Model/Material.h"

class MaterialLoader {
public:

	static Material load(const aiMaterial* aiMat, const std::string& objPath) {
		aiString aiName;
		aiMat->Get(AI_MATKEY_NAME, aiName);

		std::string name = std::string(aiName.C_Str());
		Material::MaterialData data;

		aiMat->Get(AI_MATKEY_SHININESS, data.matShininess);
		aiMat->Get(AI_MATKEY_OPACITY, data.matOpacity);

		aiColor3D color;
		aiMat->Get(AI_MATKEY_COLOR_AMBIENT, color);
		data.colorAmbient = aiCol_glmVec(color);
		aiMat->Get(AI_MATKEY_COLOR_DIFFUSE, color);
		data.colorDiffuse = aiCol_glmVec(color);
		aiMat->Get(AI_MATKEY_COLOR_SPECULAR, color);
		data.colorSpecular = aiCol_glmVec(color);
		aiMat->Get(AI_MATKEY_COLOR_EMISSIVE, color);
		aiMat->Get(AI_MATKEY_COLOR_REFLECTIVE, color);
		aiMat->Get(AI_MATKEY_COLOR_TRANSPARENT, color);

		auto mat = Material(name, data);
		createTextureLocators(mat, aiMat, objPath);
		return mat;
	}


private:
	static void createTextureLocators(Material& mat, const aiMaterial* aiMat, const std::string& objPath) {
		size_t index = objPath.find_last_of('/');
		std::string folderPath = index != std::string::npos ? objPath.substr(0, index) + "/" : ".";

		for (size_t i = static_cast<size_t>(aiTextureType_NONE); i < static_cast<size_t>(aiTextureType_UNKNOWN) + 1; i++) {
			aiTextureType type = static_cast<aiTextureType>(i);

			for (size_t j = 0; j < aiMat->GetTextureCount(type); j++) {
				aiString string;
				aiMat->GetTexture(type, j, &string);
				TextureResourceLocator loc = { std::string(folderPath + string.C_Str()), type };
				mat.putTextureLocator(type, loc);
			}
		}
	}


	static glm::vec3 aiCol_glmVec(aiColor3D& aiColor) {
		return { aiColor.r, aiColor.g, aiColor.b };
	}

};
