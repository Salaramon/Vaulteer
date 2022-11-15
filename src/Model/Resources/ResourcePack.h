#pragma once

#include <algorithm>
#include <unordered_map>

#include "Model/ModelResourceLocator.h"
#include "Model/Resources/ResourceLoader.h"
#include "Model/Textures/PackedTexture2DArray.h"

class ResourcePack {
public:
	ResourcePack() {
		std::cout << "Resource pack created." << std::endl;
	}
	~ResourcePack() {
		std::cout << "Resource pack destroyed." << std::endl;
	}

	void add(const ModelResourceLocator& modelLocator);
	void addAll(const std::vector<ModelResourceLocator>& modelLocators);

	/// Imports all added models
	void finalize();

	GLint getTextureLibraryId() const;

	ModelData* getModelByName(const std::string& modelName);
	const std::vector<ModelData*>& getAllResources();

private:
	bool finalized = false;

	std::unique_ptr<PackedTexture2DArray> textureLibrary;

	std::unordered_map<std::string, ModelResourceLocator> modelLocatorsByName;
	std::unordered_map<std::string, std::unique_ptr<ModelData>> resourcesByName;
	std::vector<ModelData*> resourceViews;
};
