#include "vpch.h"
#include "Model/Resources/ResourceLoader.h"

ModelData ResourceLoader::importModel(const std::string& objPath, int importFlags) {
	// default flags
	if (importFlags == -1)
		importFlags = aiProcess_GenNormals | aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace;

	Assimp::Importer modelImporter;
	const aiScene* scene = modelImporter.ReadFile(objPath, importFlags);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		// TODO: still need logging in static context
		std::cout << "Assimp Error: " + std::string(modelImporter.GetErrorString()) + "\n" << std::endl;
	}

	size_t index = objPath.find_last_of('/');
	std::string folderPath = index != std::string::npos ? objPath.substr(0, index) + "/" : ".";

	std::vector<Material> meshMaterials;
	for (int i = 0; i < scene->mNumMaterials; i++) {
		aiMaterial* aiMaterial = scene->mMaterials[i];
		Material mat(aiMaterial, folderPath);

		materialKeysByIndex.push_back(mat.name);
		materialLibrary[mat.name] = std::move(mat);
	}

	std::vector<Mesh> meshes;
	if (scene) {
		processNode(meshes, scene, scene->mRootNode);
	}

	numMaterials = materialKeysByIndex.size();
	numModels++;

	return { objPath, meshes };
}

std::unordered_map<std::string, Material>& ResourceLoader::getMaterialLibrary() {
	return materialLibrary;
}

//void ResourceLoader::processNode(std::vector<Mesh>& meshes, std::vector<Material>& materials, const aiScene* scene, aiNode* node) {
void ResourceLoader::processNode(std::vector<Mesh>& meshes, const aiScene* scene, aiNode* node) {

	for (size_t i = 0; i < node->mNumMeshes; i++) {
		aiMesh* aiMesh = scene->mMeshes[node->mMeshes[i]];

		//meshes.emplace_back(std::move(processMesh(materials, scene, aiMesh)));
		meshes.emplace_back(std::move(processMesh(scene, aiMesh)));
	}

	for (size_t i = 0; i < node->mNumChildren; i++) {
		//processNode(meshes, materials, scene, node->mChildren[i]);
		processNode(meshes, scene, node->mChildren[i]);
	}
}

//Mesh ResourceLoader::processMesh(std::vector<Material>& materials, const aiScene* scene, aiMesh* mesh) {
Mesh ResourceLoader::processMesh(const aiScene* scene, aiMesh* mesh) {
	Vertices vertices;
	Indices indices;
	std::vector<Texture> textures;

	for (size_t i = 0; i < mesh->mNumVertices; i++) {
		Vertex vertex;
		vertex.aPos = ai_glmVec(mesh->mVertices[i]);

		if (mesh->HasNormals()) {
			vertex.aNormal = ai_glmVec(mesh->mNormals[i]);
		}
		else {
			vertex.aNormal = glm::vec3(0);
		}

		if (mesh->HasTextureCoords(0)) {
			vertex.aTexCoords = { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y };
		}
		else {
			vertex.aTexCoords = glm::vec2(0.0f);
		}

		if (mesh->HasTangentsAndBitangents()) {
			vertex.aTangent = ai_glmVec(mesh->mTangents[i]);
			vertex.aBitangent = ai_glmVec(mesh->mBitangents[i]);
		}
		else {
			vertex.aTangent = glm::vec3(0);
			vertex.aBitangent = glm::vec3(0);
		}

		vertex.aModelNumber = numModels;

		// random
		//vertex.shininess = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		//vertex.c = glm::fvec3(rand() % 2, rand() % 2, rand() % 2);

		vertices.push_back(vertex);
	}


	for (size_t i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		for (size_t j = 0; j < face.mNumIndices; j++) {
			indices.push_back((face.mIndices[j]));
		}
	}

	return {vertices, indices, materialLibrary[materialKeysByIndex.at(numMaterials + mesh->mMaterialIndex)]};
}

glm::vec3 ResourceLoader::ai_glmVec(aiVector3D aiVec) {
	return { aiVec.x, aiVec.y, aiVec.z };
}
