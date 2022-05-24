#include "vpch.h"
#include "ResourceManager.h"

ResourcePack& ResourceManager::createPack(std::vector<ModelResourceLocator> resources) {
    loadedResourcePacks.push_back(std::make_unique<ResourcePack>(resourceLoader));
    ResourcePack& pack = *loadedResourcePacks.back().get();

    pack.addAll(resources);
    pack.finalize();

    return pack;
}

ResourcePack& ResourceManager::getPack(size_t index) {
    return *loadedResourcePacks.at(index);
}
