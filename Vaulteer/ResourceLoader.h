#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>

#include "ModelData.h"

class ResourceLoader {
	std::vector<aiTextureType> renderable_texture_types = {
		aiTextureType_DIFFUSE,
		aiTextureType_SPECULAR,
		aiTextureType_HEIGHT
	};

public:
	ModelData importModel(std::string objPath, int importFlags = -1);

	std::vector<Material>& getMaterialLibrary();

private:
	//void processNode(std::vector<Mesh>& meshes, std::vector<Material>& materials, const aiScene* scene, aiNode* node);
	void processNode(std::vector<Mesh>& meshes, const aiScene* scene, aiNode* node);

	//Mesh processMesh(std::vector<Material>& materials, const aiScene* scene, aiMesh* mesh);
	Mesh processMesh(const aiScene* scene, aiMesh* mesh);

	Material createMaterial(std::string folderPath, aiMaterial* material);

	glm::vec3 ai_glmVec(aiVector3D aiVec);


	std::vector<Material> materialLibrary;
	int numMaterials = 0;
};
