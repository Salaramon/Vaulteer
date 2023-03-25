#pragma once

#include <array>

#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>

#include "Model/Material.h"

constexpr size_t max_material_count = 128;

class MaterialLibrary {
public:
	// returns pointer to material, inserted or existing; material will have library index set
	static Material* create(aiMaterial* aiMaterial, const std::string& objPath) {
		
		std::unique_ptr<Material> matPtr = std::make_unique<Material>(aiMaterial, objPath);
		auto [it, inserted] = materialsByName.try_emplace(matPtr->name, std::move(matPtr));

		if (inserted) {
			//materialIndexUsed[numMaterials] = true;
			//materialKeysByIndex[numMaterials] = matPtr->name;
			//matPtr->setMaterialIndex(numMaterials++);
		}
		else {
			std::cout << std::format("Material name conflict <{}> for resource {}, material not inserted",
				aiMaterial->GetName().C_Str(), objPath) << std::endl;
		}
		return matPtr.get();
	}

	static Material* get(unsigned int index) {
		auto& name = materialKeysByIndex[index];
		return materialsByName.at(name).get();
	}

	static Material* get(const std::string& name) {
		return materialsByName.at(name).get();
	}

	static const std::map<std::string, std::unique_ptr<Material>>& getAllMaterials() {
		return materialsByName;
	}

	static size_t size() {
		return numMaterials;
	}

private:

	inline static std::array<bool, max_material_count> materialIndexUsed;
	inline static std::array<std::string, max_material_count> materialKeysByIndex;
	inline static std::map<std::string, std::unique_ptr<Material>> materialsByName;

	inline static size_t numMaterials = 0;
	
	// this class is static only
	MaterialLibrary() = default;
};
