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
		Log::trace("Resource pack created.");
	}
	~ResourcePack() {
		Log::info("Resource pack destroyed.");
	}

	void add(const ModelResourceLocator& modelLocator) {
		modelLocators.push_back(modelLocator);
	}

	void addAll(const std::vector<ModelResourceLocator>& locators) {
		modelLocators = locators;
	}

	std::vector<Mesh*> getMeshes(const std::string& name) {
		auto& vec = meshesByModelName.at(name);
		return vec;
	}

	/// Imports all added models
	void finalize() {
		if (finalized) {
			Log::warn("Attempted to finalize resource pack that has already been finalized.");
			return;
		}

		// import all models
		for (auto& locator : modelLocators) {
			// TODO debug: might not need adjacency for all objects
			locator.useAdjacency = true;

			auto meshes = ResourceLoader::importModel(locator);
			meshesByModelName[locator.name] = meshes;
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
			KYSE_ASSERT(false, std::string("Resource with name not part of pack: " + modelName));
		}
		return search->second.get();
	}*/

};
