#pragma once

#include <unordered_map>

#include <assimp/scene.h>

#include "Texture2DArray.h"

using Material = std::unordered_map<aiTextureType, Texture2DArray::TextureResourceLocator>; // textureTypeLocators;

