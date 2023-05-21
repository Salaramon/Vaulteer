#pragma once

#include <array>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>

#include "MaterialLoader.h"
#include "Model/Material.h"


constexpr size_t max_material_count = 128;

class MaterialLibrary {
public:
	static Material* get(unsigned int index) {
		return materialLibrary[index].get();
	}

	static Material* get(const std::string& name) {
		return get(materialIndexByName.at(name));
	}

	static Material* create(Material::MaterialData& data, const std::string& name) {
		if (Material* exists = find(name)) {
			std::cout << std::format("Material name conflict <{}>, material not inserted", name) << std::endl;
			return exists;
		}

		auto mat = std::make_unique<Material>(name, data);
		mat->setMaterialIndex(numMaterials);

		Material* matPtr = mat.get();
		materialLibrary.push_back(std::move(mat));
		materialIndexByName[matPtr->name] = numMaterials++;
		
		return matPtr;
	}

	// returns pointer to material, inserted or existing; material will have library index set
	static Material* create(aiMaterial* aiMat, const std::string& objPath) {
		if (Material* exists = find(aiMat->GetName().C_Str())) {
			std::cout << std::format("Material name conflict <{}> for resource {}, material not inserted",
				aiMat->GetName().C_Str(), objPath) << std::endl;
			return exists;
		}
		
		auto mat = std::make_unique<Material>(MaterialLoader::load(aiMat, objPath));
		mat->setMaterialIndex(numMaterials);

		Material* matPtr = mat.get();
		materialLibrary.push_back(std::move(mat));
		materialIndexByName[matPtr->name] = numMaterials++;
		
		return matPtr;
	}

	static std::vector<Material*> getAllMaterials() {
		std::vector<Material*> materials;
		for (auto& mat : materialLibrary) {
			materials.push_back(mat.get());
		}
		return materials;
	}

	static std::array<Material::MaterialData, max_material_count> getMaterialData() {
		std::array<Material::MaterialData, max_material_count> materials;
		for (auto& mat : materialLibrary) {
			materials[mat->materialIndex] = mat.get()->data;
		}
		return materials;
	}

	static size_t size() {
		return numMaterials;
	}

	static Material* find(const std::string& name) {
		auto it = materialIndexByName.find(name);
		if (it != materialIndexByName.end()) {
			return materialLibrary[(*it).second].get();
		}
		return nullptr;
	}
	 
private:
	inline static std::unordered_map<std::string, size_t> materialIndexByName;
	inline static std::vector<std::unique_ptr<Material>> materialLibrary;

	inline static size_t numMaterials = 0;
	
	// this class is static only
	MaterialLibrary() = default;
};
