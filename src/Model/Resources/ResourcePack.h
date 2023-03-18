#pragma once

#include <algorithm>
#include <unordered_map>
#include <tuple>

#include "Model/ModelResourceLocator.h"
#include "Model/Resources/ResourceLoader.h"
#include "Model/Textures/PackedTexture2DArray.h"
#include "Model/Mesh.h"

class ResourcePack {
public:
	ResourcePack() {
		std::cout << "Resource pack created." << std::endl;
	}
	~ResourcePack() {
		std::cout << "Resource pack destroyed." << std::endl;
	}

	void add(const ModelResourceLocator& modelLocator) {
		modelLocatorsByName[modelLocator.name] = modelLocator;
	}
	void addAll(const std::vector<ModelResourceLocator>& modelLocators) {
		for (auto& modelLocator : modelLocators)
			add(modelLocator);
	}

	/// Imports all added models
	void finalize() {
		if (finalized) {
			std::cout << "Attempted to finalize resource pack that has already been finalized." << std::endl;
			return;
		}

		// import all models
		for (const auto& resource : modelLocatorsByName) {
			meshes = ResourceLoader::importModel(resource.second);
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
		for (Mesh* mesh : meshes) {
			//updateModelDataWithTextureUnits(*modelData, *textureLibrary);
		}

		finalized = true;
	}

	GLint getTextureLibraryId() const {
		return textureLibrary->getTextureID();
	};

	std::vector<Mesh>* getModelByName(const std::string& modelName) {
		/*
		auto search = resourcesByName.find(modelName);
		if (search == resourcesByName.end()) {
			assert(false, std::string("Resource with name not part of pack: " + modelName));
		}
		return search->second.get();
		*/

		return nullptr;
	}


private:
	using ResourceData = std::tuple<Mesh, Material>;

	bool finalized = false;

	std::unique_ptr<PackedTexture2DArray> textureLibrary;

	std::unordered_map<std::string, ModelResourceLocator> modelLocatorsByName;
	std::vector<Mesh*> meshes;
};
