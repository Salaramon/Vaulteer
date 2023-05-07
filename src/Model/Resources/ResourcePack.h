#pragma once

#include <unordered_map>
#include <tuple>

#include "Model/Resources/ResourceLoader.h"
#include "Model/Resources/TextureLibrary.h"
#include "Model/Mesh.h"
#include "Model/Textures/PackedTexture2DArray.h"

class ResourcePack {
public:
	ResourcePack() {
		std::cout << "Resource pack created." << std::endl;
	}
	~ResourcePack() {
		std::cout << "Resource pack destroyed." << std::endl;
	}

	void add(const ModelResourceLocator& modelLocator) {
		modelLocators.push_back(modelLocator);
	}

	void addAll(const std::vector<ModelResourceLocator>& locators) {
		modelLocators = locators;
	}

	std::vector<Mesh*>& getMeshes(const std::string& name) {
		return meshesByModelName.at(name);
	}

	/// Imports all added models
	void finalize() {
		if (finalized) {
			std::cout << "Attempted to finalize resource pack that has already been finalized." << std::endl;
			return;
		}

		// import all models
		for (const auto& locator : modelLocators) {
			meshesByModelName[locator.name] = ResourceLoader::importModel(locator);
		}

		// find all locators
		std::vector<Material> materialsWithTextures;
		for (const auto& material : MaterialLibrary::getAllMaterials()) {
			if (material && !material->textureTypeLocators.empty()) {
				materialsWithTextures.push_back(*material);
			}
		}

		// create packed texture
		// TODO: eats memory when loading backpack/models with large textures because it loads everything at once
		textureLibrary = std::make_unique<Texture2DArray>();

		// update all models' units
		for (std::vector<Mesh*> meshList : meshesByModelName | std::views::values) {
			//todo what to do with texture unit range?
			//updateModelDataWithTextureUnits(*modelData, *textureLibrary);
		}

		finalized = true;
	}

	GLint getTextureLibraryId() const {
		return textureLibrary->getTextureID();
	}

	/*std::vector<Mesh> getModelByName(const std::string& modelName) {
		auto search = resourcesByName.find(modelName);
		if (search == resourcesByName.end()) {
			assert(false, std::string("Resource with name not part of pack: " + modelName));
		}
		return search->second.get();
	}*/


private:
	using ResourceData = std::tuple<Mesh, Material>;

	bool finalized = false;

	std::vector<int> loadedTextureIds;

	std::unique_ptr<Texture2DArray> textureLibrary;

	std::vector<ModelResourceLocator> modelLocators;
	std::unordered_map<std::string, std::vector<Mesh*>> meshesByModelName;
};
