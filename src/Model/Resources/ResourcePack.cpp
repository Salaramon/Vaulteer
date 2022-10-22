#include "vpch.h"
#include "Model/Resources/ResourcePack.h"

#include <ranges>

void ResourcePack::add(const ModelResourceLocator& modelLocator) {
	modelLocatorsByName[modelLocator.name] = modelLocator;
}

void ResourcePack::addAll(const std::vector<ModelResourceLocator>& modelLocators) {
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
		auto ptr = ResourceLoader::importModel(resource.second);
		resourceViews.push_back(ptr.get());
		resourcesByName[resource.first] = std::move(ptr);
	}

	// find all locators
	std::vector<TextureResourceLocator> allLocators;
	for (const auto& materials : MaterialLibrary::getAllMaterials() | std::views::values) {
		for (const TextureResourceLocator& locators : materials->textureTypeLocators | std::views::values) {
			allLocators.push_back(locators);
			std::cout << "making texture with resource: " << locators.path << std::endl;
		}
	}

	// create packed texture
	// TODO: eats memory when loading backpack/models with large textures because it loads everything at once
	textureLibrary = std::make_unique<PackedTexture2DArray>(allLocators);

	// update all models' units
	for (std::unique_ptr<ModelData>& model : resourcesByName | std::views::values) {
		model->updateWithTextureUnits(*textureLibrary);
	}

	finalized = true;
}

GLint ResourcePack::getTextureLibraryId() const {
	return textureLibrary->getTextureID();
}

ModelData* ResourcePack::getModelByName(const std::string& modelName) {
	auto search = resourcesByName.find(modelName);
	if (search == resourcesByName.end()) {
		assert(false, std::string("Resource with name not part of pack: " + modelName));
	}
	return search->second.get();
}

const std::vector<ModelData*>& ResourcePack::getAllResources() {
	return resourceViews;
}
