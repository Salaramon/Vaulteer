#pragma once

#include <assimp/scene.h>

#include "Model/Mesh.h"
#include "Model/Material.h"
#include "Model/Resources/MeshLoader.h"

class MeshLibrary {
public:
	static std::vector<Mesh*> create(aiMesh* aiMesh, Material* mat, std::string& objPath) {
		KYSE_ASSERT(false, "Not implemented");
	}

	// creates a set of meshes with the given path as key. existing keys will return already inserted set of meshes
	static std::vector<Mesh*> create(const std::string& objPath, const aiScene* scene, std::vector<Material*> sceneMaterials) {

		auto it = meshIndexByPath.find(objPath);
		if (it != meshIndexByPath.end())
			return view(meshLibrary[(*it).second]);

		std::vector<std::unique_ptr<Mesh>> meshes;
		MeshLoader::loadFromScene(meshes, scene, sceneMaterials);

		std::vector<Mesh*> retVec = view(meshes);
		meshLibrary.push_back(std::move(meshes));

		auto [meshByPath, inserted] = meshIndexByPath.emplace(objPath, numMeshes);
		KYSE_ASSERT(inserted);

		numMeshes++;

		return retVec;
	}

	static std::vector<Mesh*> get(unsigned int index) {
		KYSE_ASSERT(index < numMeshes);
		return view(meshLibrary[index]);
	}

	static std::vector<Mesh*> get(const std::string& objPath) {
		auto it = meshIndexByPath.find(objPath);
		if (it != meshIndexByPath.end()) {
			return get(meshIndexByPath[objPath]);
		}
		KYSE_ASSERT(false, "Mesh with path does not exist in library.");
		return {};
	}

	static size_t size() {
		return meshLibrary.size();
	}

private:
	static std::vector<Mesh*> view(const std::vector<std::unique_ptr<Mesh>>& vec) {
		std::vector<Mesh*> ret;
		for (auto& ptr : vec) {
			ret.push_back(ptr.get());
		}
		return ret;
	}


	inline static std::vector<std::vector<std::unique_ptr<Mesh>>> meshLibrary;
	inline static std::unordered_map<std::string, size_t> meshIndexByPath;

	inline static size_t numMeshes = 0;

	// this class is static only
	MeshLibrary() = default;
};