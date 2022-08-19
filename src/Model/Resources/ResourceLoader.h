#pragma once

// ReSharper disable once CppUnusedIncludeDirective
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>

#include "Model/Data/ModelData.h"

class ResourceLoader {
	std::vector<aiTextureType> renderable_texture_types = {
		aiTextureType_DIFFUSE,
		aiTextureType_SPECULAR,
		aiTextureType_HEIGHT
	};

public:
	ModelData importModel(const std::string& objPath, int importFlags = -1);

	std::unordered_map<std::string, Material>& getMaterialLibrary();

private:
	//void processNode(std::vector<Mesh>& meshes, std::vector<Material>& materials, const aiScene* scene, aiNode* node);
	void processNode(std::vector<Mesh>& meshes, const aiScene* scene, aiNode* node);

	//Mesh processMesh(std::vector<Material>& materials, const aiScene* scene, aiMesh* mesh);
	Mesh processMesh(const aiScene* scene, aiMesh* mesh);

	static glm::vec3 ai_glmVec(aiVector3D aiVec);


	std::vector<std::string> materialKeysByIndex;
	std::unordered_map<std::string, Material> materialLibrary;
	size_t numMaterials = 0;

	int numModels = 0;
};
