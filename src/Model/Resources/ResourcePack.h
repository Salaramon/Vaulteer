#pragma once

#include <unordered_map>
#include <tuple>

#include "Model/Resources/ResourceLoader.h"
#include "Model/Resources/TextureLibrary.h"
#include "Model/Mesh.h"

class ResourcePack {
public:
	bool finalized = false;

	Texture2DArray* texture;

	std::vector<ModelResourceLocator> modelLocators;
	std::unordered_map<std::string, std::vector<Mesh*>> meshesByModelName;


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
		std::vector<Material*> materialsWithTextures;
		for (const auto& material : MaterialLibrary::getAllMaterials()) {
			if (material && !material->textureTypeLocators.empty()) {
				materialsWithTextures.push_back(material);
			}
		}

		// store textures -- and update textureId in materials
		texture = TextureLibrary::storeMaterialTextures(materialsWithTextures);

		finalized = true;
	}

	GLint getTextureID() const {
		if (!texture)
			return 0;
		return texture->getTextureID();
	}

	/*std::vector<Mesh> getModelByName(const std::string& modelName) {
		auto search = resourcesByName.find(modelName);
		if (search == resourcesByName.end()) {
			assert(false, std::string("Resource with name not part of pack: " + modelName));
		}
		return search->second.get();
	}*/

};
