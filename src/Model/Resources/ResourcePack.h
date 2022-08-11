#pragma once

#include <algorithm>
#include <unordered_map>

#include "Model/ModelResourceLocator.h"
#include "Model/Resources/ResourceLoader.h"
#include "Model/Textures/PackedTexture2DArray.h"

class ResourcePack {
public:
	ResourcePack(ResourceLoader& loader) : loader(loader) {
		std::cout << "Resource pack created." << std::endl;
	}
	~ResourcePack() {
		std::cout << "Resource pack destroyed." << std::endl;
	}

	void add(ModelResourceLocator modelLocator);
	void addAll(std::vector<ModelResourceLocator> modelLocators);

	/// Imports all added models
	void finalize();

	GLint getTextureLibraryId();

	ModelData* getModelByName(std::string modelName);
	const std::vector<ModelData*>& getAllResources();

private:
	ResourceLoader& loader;

	bool finalized = false;

	std::unique_ptr<PackedTexture2DArray> textureLibrary;

	std::unordered_map<std::string, ModelResourceLocator> modelLocatorsByName;
	std::unordered_map<std::string, std::unique_ptr<ModelData>> resourcesByName;
	std::vector<ModelData*> resourceViews;
};
