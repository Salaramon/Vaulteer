#pragma once

#include <algorithm>
#include <unordered_map>

#include "ModelResourceLocator.h"
#include "ResourceLoader.h"
#include "PackedTexture2DArray.h"

class ResourcePack : DebugLogger<ResourcePack> {
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
	std::vector<ModelData*> getAllItems();

private:
	ResourceLoader& loader;

	bool finalized = false;

	std::unique_ptr<PackedTexture2DArray> textureLibrary;

	std::unordered_map<std::string, ModelResourceLocator> modelLocatorsByName;
	std::unordered_map<std::string, std::unique_ptr<ModelData>> resourcesByName;
};
