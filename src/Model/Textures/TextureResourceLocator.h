#pragma once

#include <string>
#include <assimp/scene.h>

/// <summary>
/// Texture resource metadata 
/// </summary>
struct TextureResourceLocator {
    std::string path;
    aiTextureType type;
};