#include "vpch.h"
#include "Model/Resources/ResourceManager.h"

ResourcePack& ResourceManager::createPack(const std::vector<ModelResourceLocator>& resources) {
    loadedResourcePacks.push_back(std::make_unique<ResourcePack>());
    ResourcePack& pack = *loadedResourcePacks.back();

    pack.addAll(resources);
    pack.finalize();

    return pack;
}

ResourcePack& ResourceManager::getPack(size_t index) {
    return *loadedResourcePacks.at(index);
}

void ResourceManager::setMaxTextureSize(size_t maxTextureSize) {
	this->maxTextureSize = maxTextureSize;
}
void ResourceManager::setPackTextures(bool packTextures) {
	this->packTextures = packTextures;
}

