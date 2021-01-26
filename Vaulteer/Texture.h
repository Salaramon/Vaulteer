#pragma once

#include <string>

class Texture
{
public:
	enum class TextureType {
		DIFFUSE,
		SPECULAR
	};

	uint32_t id;
	TextureType type;
	std::string path;
};

