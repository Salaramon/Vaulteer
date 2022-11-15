#include "vpch.h"
#include "MaterialLibrary.h"


std::shared_ptr<Material> MaterialLibrary::create(aiMaterial* aiMaterial, const std::string& objPath) {
	auto matPtr = std::make_shared<Material>(aiMaterial, objPath);
	auto [it, inserted] = materialsByName.try_emplace(matPtr->name, matPtr);

	if (inserted) {
		materialIndexUsed[numMaterials] = true;
		materialKeysByIndex[numMaterials] = matPtr->name;
		matPtr->setMaterialIndex(numMaterials++);
	}
	else {
		std::cout << std::format("Material name conflict <{}> for resource {}, material not inserted",
			aiMaterial->GetName().C_Str(), objPath) << std::endl;
	}
	return it->second;
}

std::shared_ptr<Material> MaterialLibrary::get(unsigned int index) {
	auto& name = materialKeysByIndex[index];
	return materialsByName.at(name);
}

std::shared_ptr<Material> MaterialLibrary::get(const std::string& name) {
	return materialsByName.at(name);
}

const std::map<std::string, std::shared_ptr<Material>>& MaterialLibrary::getAllMaterials() {
	return materialsByName;
}
