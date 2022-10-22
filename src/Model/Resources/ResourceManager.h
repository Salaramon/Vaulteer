#pragma once

#include "Model/Resources/ResourcePack.h"
#include "Model/Resources/ResourceLoader.h"

class ResourceManager {
public:
	static ResourcePack& createPack(const std::vector<ModelResourceLocator>& resources);
	static ResourcePack& getPack(size_t index);
	
	// TODO these are unused
	void setMaxTextureSize(size_t maxTextureSize);
	void setPackTextures(bool packTextures);
	
private:
	inline static std::vector<std::unique_ptr<ResourcePack>> loadedResourcePacks;

	size_t maxTextureSize = 8192;
	size_t packTextures = true;

	// this class is static only
	ResourceManager() {}
};

