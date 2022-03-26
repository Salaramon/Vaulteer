#pragma once

#include "ResourcePack.h"
#include "ResourceLoader.h"

class ResourceManager {
private:
	ResourceLoader resourceLoader;
	std::vector<std::unique_ptr<ResourcePack>> loadedResourcePacks;

public:
	ResourcePack& createPack(std::vector<ModelResourceLocator> resources);
	ResourcePack& getPack(size_t index);

	void setMaxTextureSize();
	void enableTexturePacking();
};

