#include "vpch.h"
#include "Material.h"

Material::Material() :
	name("DefaultMaterial"), data(defaultMaterial) {}

Material::Material(aiMaterial* mat, std::string folderPath) {
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

inline bool Material::isTransparent() {
	return data.matOpacity < 1.0;
}

inline void Material::createTexture(aiMaterial* mat, std::string folderPath) {
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

inline glm::vec3 Material::aiCol_glmVec(aiColor3D aiColor) {
	return { aiColor.r, aiColor.g, aiColor.b };
}
