#pragma once

#include <assimp/postprocess.h>

#include "Mesh.h"
#include "PackedTexture2DArray.h"

typedef std::unordered_map<aiTextureType, std::vector<Texture2DArray::TextureUnit>> unit_by_type_map;

// Model resource metadata
struct ModelResourceLocator {
	std::string name, path;
	int importFlags = -1;
};

// Resource without ownership
/*struct ResourceView {
	Mesh* mesh = nullptr;
	unit_by_type_map unitsByType;

	ResourceView() {}
	ResourceView(OwnedResource* r) : mesh(r->mesh.get()), unitsByType(r->unitsByType) {} // removes ownership on construction
};*/
