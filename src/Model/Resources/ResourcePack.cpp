#include "vpch.h"
#include "Model/Resources/ResourcePack.h"

void ResourcePack::add(ModelResourceLocator modelLocator) {
	modelLocatorsByName[modelLocator.name] = modelLocator;
}

void ResourcePack::addAll(std::vector<ModelResourceLocator> modelLocators) {
	for (auto& modelLocator : modelLocators)
		add(modelLocator);
}

void ResourcePack::finalize() {
	if (finalized) {
		std::cout << "Attempted to finalize resource pack that has already been finalized." << std::endl;
		return;
	}

	// import all models
	for (const auto& resource : modelLocatorsByName) {
		auto ptr = std::make_unique<ModelData>(loader.importModel(resource.second.path, resource.second.importFlags));
		resourceViews.push_back(ptr.get());
		resourcesByName[resource.first] = std::move(ptr);
	}

	// find all locators
	std::vector<TextureResourceLocator> allLocators;
	for (const auto& materialEntries : loader.getMaterialLibrary()) {
		for (const auto& locatorsForMaterials : materialEntries.second.textureTypeLocators) {
			allLocators.push_back(locatorsForMaterials.second);
			std::cout << "making texture with resource: " << locatorsForMaterials.second.path << std::endl;
		}
	}

	// create packed texture
	// TODO: eats memory when loading backpack/models with large textures because it loads everything at once
	textureLibrary = std::make_unique<PackedTexture2DArray>(allLocators);
	
	// update all models' units
	for (auto& modelEntries : resourcesByName) {
		modelEntries.second->updateWithTextureUnits(*textureLibrary);
	}

	finalized = true;
}

GLint ResourcePack::getTextureLibraryId() {
	return textureLibrary.get()->getTextureID();
}

ModelData* ResourcePack::getModelByName(std::string modelName) {
	auto search = resourcesByName.find(modelName);
	if (search == resourcesByName.end()) {
		assert(std::string("Resource with name not part of pack: " + modelName).c_str());
	}
	return search->second.get();
}

const std::vector<ModelData*>& ResourcePack::getAllResources() {
	return resourceViews;
}

