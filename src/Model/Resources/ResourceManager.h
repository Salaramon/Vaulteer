#pragma once

#include "Model/Resources/ResourcePack.h"
#include "Model/Resources/ResourceLoader.h"

class ResourceManager {
	ResourceLoader resourceLoader;
	std::vector<std::unique_ptr<ResourcePack>> loadedResourcePacks;

public:
	ResourcePack& createPack(const std::vector<ModelResourceLocator>& resources);
	ResourcePack& getPack(size_t index) const;

	void setMaxTextureSize();
	void enableTexturePacking();
};

