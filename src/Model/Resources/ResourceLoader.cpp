#include "vpch.h"
#include "Model/Resources/ResourceLoader.h"

std::unique_ptr<ModelData> ResourceLoader::importModel(const ModelResourceLocator& loc) {
	return importModel(loc.path, loc.importFlags);
}

std::unique_ptr<ModelData> ResourceLoader::importModel(const std::string& objPath, int importFlags) {
	// default flags
	if (importFlags == blank_import_flags)
		importFlags = aiProcess_GenNormals | aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace;

	Assimp::Importer modelImporter;
	const aiScene* scene = modelImporter.ReadFile(objPath, importFlags);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		// TODO: still need logging in static context
		std::cout << "Assimp Error: " + std::string(modelImporter.GetErrorString()) + "\n" << std::endl;
		assert(false);
	}

	std::vector<std::shared_ptr<Material>> sceneMaterials;
	for (int i = 0; i < scene->mNumMaterials; i++) {
		aiMaterial* aiMaterial = scene->mMaterials[i];
		sceneMaterials.push_back(MaterialLibrary::create(aiMaterial, objPath));
	}

	// maybe we want to use a MeshLibrary at some point so ModelData doesn't own its meshes; it can be used here
	std::vector<Mesh> meshes;
	if (scene) {
		processNode(meshes, sceneMaterials, scene, scene->mRootNode);
	}

	numModels++;

	return std::make_unique<ModelData>(objPath, meshes);
}

void ResourceLoader::processNode(std::vector<Mesh>& meshes, std::vector<std::shared_ptr<Material>>& sceneMaterials, const aiScene* scene, const aiNode* node) {

	for (size_t i = 0; i < node->mNumMeshes; i++) {
		aiMesh* aiMesh = scene->mMeshes[node->mMeshes[i]];

		meshes.emplace_back(processMesh(sceneMaterials, aiMesh));
	}

	for (size_t i = 0; i < node->mNumChildren; i++) {
		processNode(meshes, sceneMaterials, scene, node->mChildren[i]);
	}
}

Mesh ResourceLoader::processMesh(std::vector<std::shared_ptr<Material>>& sceneMaterials, aiMesh* mesh) {
	Vertices vertices;
	Indices indices;
	std::vector<Texture> textures;
	auto mat = sceneMaterials.at(mesh->mMaterialIndex);

	for (size_t i = 0; i < mesh->mNumVertices; i++) {
		Vertex vertex;
		vertex.aPos = ai_glmVec(mesh->mVertices[i]);
		vertex.aNormal = mesh->HasNormals() ? ai_glmVec(mesh->mNormals[i]) : glm::vec3(0);
		vertex.aTexCoords = mesh->HasTextureCoords(0) ? glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y) : glm::vec2(0.0f);

		if (mesh->HasTangentsAndBitangents()) {
			vertex.aTangent = ai_glmVec(mesh->mTangents[i]);
			vertex.aBitangent = ai_glmVec(mesh->mBitangents[i]);
		}

		unsigned int firstSceneMatIndex = MaterialLibrary::size() - sceneMaterials.size();
		vertex.aMaterialNumber = mat->materialIndex;

		// random
		//vertex.shininess = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		//vertex.c = glm::fvec3(rand() % 2, rand() % 2, rand() % 2);

		vertices.push_back(vertex);
	}


	for (size_t i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		for (size_t j = 0; j < face.mNumIndices; j++) {
			indices.push_back(face.mIndices[j]);
		}
	}
	
 	return {vertices, indices, mat};
}

glm::vec3 ResourceLoader::ai_glmVec(aiVector3D aiVec) {
	return { aiVec.x, aiVec.y, aiVec.z };
}
