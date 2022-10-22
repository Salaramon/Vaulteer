#pragma once
#include "Model/Material.h"

constexpr size_t max_material_count = 128;

class MaterialLibrary {
public:
	// returns pointer to material, inserted or existing; material will have library index set
	static std::shared_ptr<Material> create(aiMaterial* aiMaterial, const std::string& objPath);

	static std::shared_ptr<Material> get(unsigned int index);
	static std::shared_ptr<Material> get(const std::string& name);

	static const std::unordered_map<std::string, std::shared_ptr<Material>>& getAllMaterials();

	static size_t size() {
		return numMaterials;
	}

private:
	inline static std::array<bool, max_material_count> materialIndexUsed;
	inline static std::array<std::string, max_material_count> materialKeysByIndex;
	inline static std::unordered_map<std::string, std::shared_ptr<Material>> materialsByName;

	inline static size_t numMaterials = 0;
	
	// this class is static only
	MaterialLibrary() = default;
};
